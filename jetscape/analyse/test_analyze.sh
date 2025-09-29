FILE_PATHS='/bundle/data/ALICE/users/jnorman/Jetscape/5020_PP_Charm_pThat3/skim/files_hadrons.txt'
# container path
CONTAINERPATH='/bundle/data/ALICE/users/jnorman/containers/local/stat_local_gcc_v5.3.sif'
# path of JETSCAPE-analysis
ANALYSIS_PATH='/user/jnorman/jetscape/JETSCAPE-analysis'


NFILES=$(wc -l < $FILE_PATHS)
echo "N files to process: ${NFILES}"

OFFSET=1
START=$OFFSET
STOP=$NFILES
STOP=3

echo "START=$START"
echo "STOP=$STOP"

for (( JOB_N = $START; JOB_N <= $STOP; JOB_N++ ))
do
  FILE=$(sed -n "$JOB_N"p $FILE_PATHS)
  source process_analyze.sh $FILE $CONTAINERPATH $ANALYSIS_PATH
done
