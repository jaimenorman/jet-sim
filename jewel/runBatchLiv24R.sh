#!/bin/bash

# Script which runs on the batch worker node, submitted via qsub (slc6) or slurm (centos7) using submitBatchLiv.sh

echo ">>> Starting Batch job on $HOSTNAME at $(date) <<<"

# set JEWEL directory
INDIR=/user/jnorman/Packages/Jewel/jewel-2.4.0

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

source /user/software/gcc/gccSetup.sh 9.3.0
source runJewelR.sh $MACRO $SEED

cp $PWD/jewel-out.log $OUTDIR/jewel-out_"$SEED".log
# comment out - don't save hepmc files
cp $PWD/jewel-out.hepmc $OUTDIR/jewel-out_"$SEED".hepmc

#source /cvmfs/alice.cern.ch/etc/login.sh
/cvmfs/alice.cern.ch/bin/alienv setenv VO_ALICE@AliPhysics::vAN-20200108_ROOT6-1,VO_ALICE@HepMC::HEPMC_02_06_10-1 -c /user/jnorman/Sim/hJet/HepMC2-repo/analyze_hepmc2_hjet $PWD/jewel-out.hepmc $PWD/output —chargedjets

#cp $PWD/output_charged.root $OUTDIR/output_charged_"$SEED".root

# Copy output files back
#cp $PWD/lz_* $OUTDIR/.

find $PWD -name "*.root" -exec cp {} $OUTDIR/output_"$SEED".root \;

# Batch clean up to remove temp dir
rm -rf $TMPDIR

echo ">>> Finishing Batch job on $HOSTNAME at $(date) <<<"
