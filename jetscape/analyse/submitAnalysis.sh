#! /bin/bash

# container path
CONTAINERPATH='/bundle/data/ALICE/users/jnorman/containers/local/stat_local_gcc_v5.3.sif'
# path of JETSCAPE-analysis
ANALYSIS_PATH='/user/jnorman/jetscape/JETSCAPE-analysis'
# path containing final_state_hadron files
BASE_DIR='/bundle/data/ALICE/users/jnorman/Jetscape/5020_PP_Charm_pThat3/skim'

# generate file with file names if it doesn't already exist
FILE_LIST="$BASE_DIR/files_hadrons.txt"
if [ ! -e "$FILE_LIST" ]; then
    echo "Generating file list at $FILE_LIST..."
    find "$BASE_DIR" -type f -name 'jetscape*final_state_hadrons*.parquet' > "$FILE_LIST"
else
    echo "File list $FILE_LIST already exists..."
fi

NFILES=$(wc -l < $FILE_LIST)
echo "N files to process: ${NFILES}"

N_FILES_PER_JOB=10
OFFSET=1
START=$OFFSET
#STOP=$NFILES
STOP=4

echo "START=$START"
echo "STOP=$STOP"

REMAINDER=$(($STOP%$N_FILES_PER_JOB))

# new
for (( JOB_N = $(($START-1)); JOB_N <= $(($STOP/N_FILES_PER_JOB)); JOB_N++ ))
do
  START_JOB=$(($(($JOB_N*$N_FILES_PER_JOB))+1))
  STOP_JOB=$(($(($JOB_N*$N_FILES_PER_JOB))+$N_FILES_PER_JOB))
  if (($JOB_N == $(($STOP/N_FILES_PER_JOB)))); then
      STOP_JOB=$(($(($JOB_N*$N_FILES_PER_JOB))+$REMAINDER))
  fi
  sbatch -c 1 --export=FILE_LIST=$FILE_LIST,START_JOB=$START_JOB,STOP_JOB=$STOP_JOB,CONTAINERPATH=$CONTAINERPATH,ANALYSIS_PATH=$ANALYSIS_PATH combine_analyze.sh 
done
