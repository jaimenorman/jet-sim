# script used to run JEWEL within a batch job
#
# this script needs to go in the JEWEL base directory
#
# use a parameter file which has NJOB set to MYNJOB
# to ensure that the seed for each job is different
# (may be a better way to do this

#PARAMSFILE=params.5020.0-10.batch.dat
#PARAMSFILERUN=params.5020.0-10.batch."$2".dat
#PARAMSFILE=params.5020.0-10.batch.dat
#PARAMSFILERUN=params.5020.0-10.batch."$2".dat
PARAMSFILE=params.5020.vac.dat
PARAMSFILERUN=params.5020.vac.$"$2".dat

echo in directory $PWD to run JEWEL with params file $PARAMSFILE giving macro $1 and seed $2
cp $PARAMSFILE $PARAMSFILERUN
perl -pi -e 's/MYNJOB/'$2'/g' $PARAMSFILERUN
#cat $PARAMSFILERUN
./jewel-2.2.0-vac $PARAMSFILERUN
rm $PARAMSFILERUN
echo end run script
