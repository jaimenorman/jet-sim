# submit job script to batch via slurm

# Variables to control job loop
# number to start at
OFFSET=0
# number of parallel jobs to run
JOBS=100

# Outdir labelled by date and time
OUTDIR=/scratch/jnorman/JETSCAPE/$(date +%Y%m%d.%H%M)
# job name - files will be output to this name
MACRO='jetscape-pp'

mkdir $OUTDIR

echo "Submitting $JOBS jobs with $MACRO ..."

# Do a lop over the different jobs you want to run
for ((i=$OFFSET; i< $OFFSET + $JOBS; i++))
do
    echo "... $i"
    JOBNAME=$(echo $MACRO | cut -d '.' -f 1)".$i"

	# Submit from CC7 or AL9, seeting the queue (-p), output and error files (-o and -r), and shell vars to pass to the script running on the notes (--export)
	# Check jobs with squeue
	# Cancel jobs with scancel
	sbatch -J $JOBNAME -p compute -o $OUTDIR/$JOBNAME.out -e $OUTDIR/$JOBNAME.err --export=MACRO=$MACRO,OUTDIR=$OUTDIR,SEED=$i runBatchLiv.sh
done

