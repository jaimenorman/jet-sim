#! /bin/bash

for (( JOB_N = $START_JOB; JOB_N <= $STOP_JOB; JOB_N++ ))
do
    echo job n = $JOB_N
    FILE=$(sed -n "$JOB_N"p $FILE_LIST)
    source process_analyze.sh $FILE $CONTAINERPATH $ANALYSIS_PATH
done
