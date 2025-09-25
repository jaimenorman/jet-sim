#! /bin/bash

# submit slurm jobs to compress JETSCAPE final_state_hadron files into .parquet files
# here a list of the output files is created and then each job compresses
# a number of files, by launching the 'combine_skim.sh' script  per job
# which then runs multiple file compressions

DATE_TIME=$(date +%Y%m%d.%H%M)
# --- options to change ---
# container path
CONTAINERPATH='/bundle/data/ALICE/users/jnorman/containers/local/stat_local_gcc_v5.3.sif'
# path of JETSCAPE-analysis
ANALYSIS_PATH='/user/jnorman/jetscape/JETSCAPE-analysis'
# path containing final_state_hadron files
BASE_DIR='/bundle/data/ALICE/users/jnorman/Jetscape/5020_PP_Charm_pThat3'
# number of events to save per parquet file 
N_EVENTS_PER_FILE=20000
# number of files to skim per job
N_FILES_PER_JOB=10
OFFSET=1

# generate file with file names if it doesn't already exist
FILE_LIST="$BASE_DIR/files_hadrons.txt"
if [ ! -e "$FILE_LIST" ]; then
    echo "Generating file list at $FILE_LIST..."
    find "$BASE_DIR" -type f -name 'jetscape*final_state_hadrons.dat' > "$FILE_LIST"
else
    echo "File list $FILE_LIST already exists..."
fi

# now run
NFILES=$(wc -l < $FILE_LIST)
echo "N files to process: ${NFILES}"

START=$OFFSET
STOP=$NFILES

echo "START=$START"
echo "STOP=$STOP"

REMAINDER=$(($STOP%$N_FILES_PER_JOB))

for (( JOB_N = $(($START-1)); JOB_N <= $(($STOP/N_FILES_PER_JOB)); JOB_N++ ))
do
  START_JOB=$(($(($JOB_N*$N_FILES_PER_JOB))+1))
  STOP_JOB=$(($(($JOB_N*$N_FILES_PER_JOB))+$N_FILES_PER_JOB))
  if (($JOB_N == $(($STOP/N_FILES_PER_JOB)))); then
      STOP_JOB=$(($(($JOB_N*$N_FILES_PER_JOB))+$REMAINDER))
  fi
  sbatch --export=FILE_LIST=$FILE_LIST,START_JOB=$START_JOB,STOP_JOB=$STOP_JOB,N_EVENTS_PER_FILE=$N_EVENTS_PER_FILE,DATE_TIME=$DATE_TIME,CONTAINERPATH=$CONTAINERPATH,ANALYSIS_PATH=$ANALYSISPATH combine_skim.sh 
done




