#! /bin/bash

# This script takes an input file path as an argument, and runs a python script to 
# process the input file and write an output ROOT file.
# The main use is to give this script to a slurm script.


INPUT_FILE=$1
CONTAINERPATH=$2
ANALYSIS_PATH=$3

# Define output path from relevant sub-path of input file

FILE_NAME=$(echo $INPUT_FILE | rev | cut -d '/' -f1 | rev) # get .parquet event file name
PREPEND=$(echo $INPUT_FILE| sed "s/$FILE_NAME//") # get path where file is
OUTPUT_DIR="$PREPEND/histograms" #define path for histogram output relative to path of file
mkdir -p $OUTPUT_DIR

#make output file of one the input file of the other
string_old="final_state_hadrons"
string_new="observables"
FILE_NAME_HIST=$(echo $FILE_NAME | sed "s/$string_old/$string_new/")

OUTPUT_DIR_FILE="$OUTPUT_DIR/$FILE_NAME_HIST"

echo input event file = $INPUT_FILE
echo output analysis directory = $OUTPUT_DIR

echo singularity exec --bind /bundle $CONTAINERPATH $PWD/analyze_djet.sh $INPUT_FILE $OUTPUT_DIR $OUTPUT_DIR_FILE $ANALYSIS_PATH
singularity exec --bind /bundle $CONTAINERPATH $PWD/analyze_djet.sh $INPUT_FILE $OUTPUT_DIR $OUTPUT_DIR_FILE $ANALYSIS_PATH
