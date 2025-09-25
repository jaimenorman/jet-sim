# submit job script to batch via slurm 

# Variables to control job loop
# number to start at
OFFSET=0
# number of parallel jobs to run
JOBS=100

# Outdir labelled by date and time
OUTDIR=/scratch/jnorman/JETSCAPE/$(date +%Y%m%d.%H%M)
# job name - files will be output to this name
MACRO='jetscape-AuAu'

mkdir $OUTDIR

# format hydro files for each job
#HYDROBASEDIR='/bundle/data/ALICE/users/jnorman/Jetscape/Production_Runs_for_Quenching/install/5020'
HYDROBASEDIR='/bundle/data/ALICE/users/jnorman/Jetscape/Production_Runs_for_Quenching/install/200'
CENTMIN=0
CENTMAX=10
FILEMIN=0
FILEMAX=10

NITERATIONS=$(( ($JOBS-1) / ( ($FILEMAX-$FILEMIN)*($CENTMAX-$CENTMIN) ) + 1 ))

echo n iterations = $NITERATIONS

# Generate all unique combinations
combinations=()
count_combinations=0
for ((i=0; i<NITERATIONS; i++)); do
    for ((centPick=CENTMIN; centPick<CENTMAX; centPick++)); do
        for ((evPick=FILEMIN; evPick<FILEMAX; evPick++)); do

            printf -v centPickSave '%0'2'd' $((centPick+10))
            printf -v evPickSave '%0'2'd' $evPick
            combinations+=("$centPickSave$evPickSave")

        done
    done
done

# Shuffle the combinations
shuf_combinations=($(shuf -e "${combinations[@]}"))

echo "Submitting $JOBS jobs with $MACRO ..."

# Do a loop over the different jobs you want to run
for ((i=$OFFSET; i< $OFFSET + $JOBS; i++))
do
    # chose hydro file
    pair="${shuf_combinations[i]}"
    centPick=${pair:0:2}
    evPick=${pair:2}

    printf -v centPathMin '%0'2'd' $((10#$centPick - 10))
    printf -v centPathMax '%0'2'd' $((10#$centPick - 9))
    HYDROPATHNAME="$HYDROBASEDIR"/cent_"$centPathMin"_"$centPathMax"/$evPick

    #submit jobs
    echo "... $i"
    JOBNAME=$(echo $MACRO | cut -d '.' -f 1)".$i"

	# Submit from CC7 or AL9, seeting the queue (-p), output and error files (-o and -r), and shell vars to pass to the script running on the notes (--export)
	# Check jobs with squeue
	# Cancel jobs with scancel
	sbatch -J $JOBNAME -p compute -o $OUTDIR/$JOBNAME.out -e $OUTDIR/$JOBNAME.err --export=MACRO=$MACRO,OUTDIR=$OUTDIR,SEED=$i,HYDROPATHNAME=$HYDROPATHNAME runBatchLiv.sh
done

