# submit job script to batch via qsub (slc6) or slurm (centos7)

# Variables to control job loop
# number to start at
OFFSET=0
# number of parallel jobs to run
JOBS=10

# Outdir labelled by date and time
OUTDIR=/scratch/jnorman/JEWEL/$(date +%Y%m%d.%H%M)
# job name - files will be output to this name
MACRO='jewel'

mkdir $OUTDIR

# Work out automatically the OS
SLURM=$([[ -f /etc/redhat-release ]] && cat /etc/redhat-release | grep -c CentOS)



echo "Submitting $JOBS jobs with $MACRO ..."

# Do a lop over the different jobs you want to run
for ((i=$OFFSET; i< $OFFSET + $JOBS; i++))
do
    echo "... $i"
    JOBNAME=$(echo $MACRO | cut -d '.' -f 1)".$i"
    if [[ $SLURM -eq 1 ]] 
    then
	# Submit from CC7, seeting the queue (-p), output and error files (-o and -r), and shell vars to pass to the script running on the notes (--export)
	# Check jobs with squeue
	# Cancel jobs with scancel
	sbatch -J $JOBNAME -p compute -o $OUTDIR/$JOBNAME.out -e $OUTDIR/$JOBNAME.err --export=MACRO=$MACRO,OUTDIR=$OUTDIR,SEED=$i runBatchLiv24R.sh
    else
	# Submit from SLC6, seeting the queue (-q), output and error files (-o and -r), and shell vars to pass to the script running on the notes (-v)
	# Check jobs with qstat
	# Cancel jobs with qdel
	qsub -N $JOBNAME -q medium64 -o $OUTDIR/$JOBNAME.out -e $OUTDIR/$JOBNAME.err -v MACRO=$MACRO,OUTDIR=$OUTDIR,SEED=$i runBatchLiv24R.sh
    fi    
done

