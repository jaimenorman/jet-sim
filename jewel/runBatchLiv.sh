#!/bin/bash

# Script which runs on the batch worker node, submitted via qsub (slc6) or slurm (centos7) using submitBatchLiv.sh
run_script=runJewelsimple.sh

echo ">>> Starting Batch job on $HOSTNAME at $(date) <<<"

# set JEWEL directory
INDIR=/user/djones/Packages/jewel-2.4.0

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

source runJewelsimple.sh $MACRO $SEED

cp $PWD/jewel-out.log $OUTDIR/jewel-out_"$SEED".log
cp $PWD/jewel-out.hepmc $OUTDIR/jewel-out_"$SEED".hepmc

#source $run_script $MACRO $SEED

#source /cvmfs/alice.cern.ch/etc/login.sh
#/cvmfs/alice.cern.ch/bin/alienv setenv VO_ALICE@AliPhysics::vAN-20200108_ROOT6-1,VO_ALICE@HepMC::HEPMC_02_06_10-1 -c /user/djones/simulation/jet-sim/analyze_hepmc2_hjet $PWD/jewel-out.hepmc $PWD/output —chargedjets

# Copy output files back
#cp $PWD/lz_* $OUTDIR/.

#find $PWD -name "*.root" -exec cp {} $OUTDIR/Vaccum_"$SEED".root \;

# Batch clean up to remove temp dir
rm -rf $TMPDIR

echo ">>> Finishing Batch job on $HOSTNAME at $(date) <<<"
