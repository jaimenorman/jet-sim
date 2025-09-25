#!/bin/bash

# Script which runs on the batch worker node, submitted via qsub (slc6) or slurm (centos7) using submitBatchLiv.sh

echo ">>> Starting Batch job on $HOSTNAME at $(date) <<<"

# the following paths should be set according to users setup
#
# set JETSCAPE directory
INDIR=/user/jnorman/jetscape/JETSCAPE-COMP2
# container path
CONTAINERPATH=/user/jnorman/jetscape/jetbox
# parameter file
PARAMSFILE=/user/jnorman/jetscape-user/generate/jetscape_user_PP19_charm.xml
#PARAMSFILE=/user/jnorman/jetscape-user/generate/jetscape_user_PP19_nocharm.xml
#PARAMSFILE=/user/jnorman/jetscape-user/generate/jetscape_user_pbpb_502TeV_gridHydro_charm.xml

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
cd build

# setup parameter file and run

PARAMSFILERUN=./jetscape_user_"$SEED".xml

echo in directory $PWD to run JEWEL with params file $PARAMSFILE giving seed $SEED
cp $PARAMSFILE $PARAMSFILERUN


echo list current directory
ls ./
echo list back directory
ls ../config/

#
# modify parameter file
#
# set seed
digit=`printf "%04d" $SEED`
perl -pi -e 's/MYNJOB/'$digit'/g' $PARAMSFILERUN
perl -pi -e 's/MYSEED/'$SEED'/g' $PARAMSFILERUN

# set hydro path
if [[ -v HYDROPATHNAME ]]
then
    echo run AA mode - set hydro path name as $HYDROPATHNAME
    HYDROPATHNAMEREPLACE=$(echo $HYDROPATHNAME | sed -r 's/\//\\\//g')
    perl -pi -e 's/MYHYDROPATH/'$HYDROPATHNAMEREPLACE'/g' $PARAMSFILERUN
else
    echo run pp mode - no hydro path set up
fi

echo print params file $PARAMSFILERUN
cat $PARAMSFILERUN
singularity exec --bind /bundle $CONTAINERPATH  ./runJetscape $PARAMSFILERUN

# copy output files

ls ./

cp jetscape_* $OUTDIR/
cp $PARAMSFILERUN $OUTDIR/
cp *.xml $OUTDIR/
cp *.yaml $OUTDIR/

# Batch clean up to remove temp dir
rm -rf $TMPDIR

echo ">>> Finishing Batch job on $HOSTNAME at $(date) <<<"
