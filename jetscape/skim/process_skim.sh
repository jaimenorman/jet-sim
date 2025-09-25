#! /bin/bash

# This script takes an input file path as an argument, and runs a python script to 
# process the input file and write an output parquet file
# The main use is to give this script to a slurm script.

# passed from script
INPUT_FILE=$1
N_EV_PER_FILE=$2
DATE_TIME=$3
ANALYSIS_PATH=$4

# Define output path from relevant sub-path of input file
# Note: suffix depends on file structure of input file -- need to edit appropriately

FILE_NAME=$(echo $INPUT_FILE | rev | cut -d '/' -f1 | rev) # get .parquet event file name
PREPEND=$(echo $INPUT_FILE| sed "s/$FILE_NAME//") # get path where file is
FILE_NAME_NO_FORMAT=$(echo $FILE_NAME| sed "s/.dat//") # get path where file is
OUTPUT_DIR="$PREPEND/skim/$FILE_NAME_NO_FORMAT".parquet #define path for histogram output relative to path of file

echo "Output dir: $OUTPUT_DIR"
echo "file name: $FILE_NAME"
echo "file name no format: $FILE_NAME_NO_FORMAT"

# Load modules
. /usr/local/init/profile.sh
module use /jetscapeOpt/heppy/modules
module load heppy/1.0
module list

# Run python script via pipenv
cd $ANALYSIS_PATH
python3 -m jetscape_analysis.analysis.reader.skim_ascii -i $INPUT_FILE -o $OUTPUT_DIR -n $N_EV_PER_FILE
cd -
