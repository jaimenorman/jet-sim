#!/bin/bash

# Script which runs on the batch worker node, submitted via qsub (slc6) or slurm (centos7) using submitBatchLiv.sh

echo ">>> Starting Batch job on $HOSTNAME at $(date) <<<"

# set JEWEL directory
INDIR=/user/jnorman/Packages/Jewel/jewel-2.2.0

SLURM=$([[ -f /etc/redhat-release ]] && cat /etc/redhat-release | grep -c CentOS)
[[ $SLURM -eq 1 ]] && TMPDIR=$HEPTMP/$SLURM_JOBID || TMPDIR=$HEPTMP/$(echo $PBS_JOBID | cut -d . -f 1)

# Make temp dir in which  to run
if [[ ! -d $TMPDIR ]]; then
    echo "Making directory $TMPDIR"
    mkdir $TMPDIR
else
    echo "Directory $TMPDIR exists"
fi

cd $TMPDIR

# Copy input files
cp -r $INDIR/* $PWD/.

# Run your job
source runJewel.sh $MACRO $SEED

# Copy output files back
#cp $PWD/lz_* $OUTDIR/.
cp $PWD/jewel-out.log $OUTDIR/jewel-out_"$SEED".log
cp $PWD/jewel-out.hepmc $OUTDIR/jewel-out_"$SEED".hepmc

# Batch clean up to remove temp dir
rm -rf $TMPDIR

echo ">>> Finishing Batch job on $HOSTNAME at $(date) <<<"
