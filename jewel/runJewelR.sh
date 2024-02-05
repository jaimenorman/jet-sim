#PARAMSFILE=params.5020.0-10.batch.dat
#PARAMSFILERUN=params.5020.0-10.batch."$2".dat
PARAMSFILE=params.5020.0-10.batch.dat
PARAMSFILERUN=params.5020.0-10.batch."$2".dat
#PARAMSFILE=params.5020.vac.simple.dat
#PARAMSFILERUN=params.5020.vac.simple."$2".dat
#PARAMSFILE=params.200.0-15.R.batch.dat
#PARAMSFILERUN=params.200.0-15.R.batch."$2".dat

echo in directory $PWD to run JEWEL with params file $PARAMSFILE giving macro $1 and seed $2
cp $PARAMSFILE $PARAMSFILERUN
perl -pi -e 's/MYNJOB/'$2'/g' $PARAMSFILERUN
echo params file to run = $PARAMSFILERUN
cat $PARAMSFILERUN
#./jewel-2.2.0-simple $PARAMSFILERUN
echo running ./jewel-2.4.0-simple $PARAMSFILERUN
./jewel-2.4.0-simple $PARAMSFILERUN
rm $PARAMSFILERUN
echo end run script
