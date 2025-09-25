#! /bin/bash
# Command line arguments

echo $PWD

for (( JOB_N=$START_JOB; JOB_N<=$STOP_JOB; JOB_N++ ))
do
  echo job n = $JOB_N
  FILE=$(sed -n "$JOB_N"p $FILE_LIST)
  echo singularity exec --bind /bundle $CONTAINERPATH source $MYPWD/process_skim.sh $FILE $N_EVENTS_PER_FILE $DATE_TIME
  singularity exec --bind /bundle $CONTAINERPATH ./process_skim.sh $FILE $N_EVENTS_PER_FILE $DATE_TIME $ANALYSIS_PATH
done
