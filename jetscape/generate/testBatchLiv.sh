# script to test running without having to submit slurm jobs

OUTDIR=/user/jnorman/jetscape-user/generate_slurm/outputTestBatchAuAu
MACRO='jetscape'
SEED=1
JOBNAME=$(echo $MACRO | cut -d '.' -f 1)".$SEED"
mkdir $OUTDIR
source runBatchLiv.sh -o $OUTDIR/$JOBNAME.out -e $OUTDIR/$JOBNAME.err --export=MACRO=$MACRO,OUTDIR=$OUTDIR,SEED=$i 

