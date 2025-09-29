#! /bin/bash

INPUT_FILE=$1
OUTPUT_DIR=$2
OUTPUT_DIR_FILE=$3
ANALYSIS_PATH=$4

# Load modules
. /usr/local/init/profile.sh
module use /jetscapeOpt/heppy/modules
module load heppy/1.0
module list

cd $ANALYSIS_PATH

echo python3 jetscape_analysis/analysis/analyze_events_djet.py -c config/STAT_5020_djet.yaml -i $INPUT_FILE -o $OUTPUT_DIR # new, using STAT
python3 jetscape_analysis/analysis/analyze_events_djet.py -c config/STAT_5020_djet.yaml -i $INPUT_FILE -o $OUTPUT_DIR # new, using STAT
