# JETSCAPE submission tools to run on Liverpool HEP systems

## generate

generate JETSCAPE events in parallel jobs using slurm

run submit script submitBatchLiv.sh for pp events or submitBatchLivHydro.sh for AA events, which then launches runBatchLiv.sh to generate events

some some user-dependent settings required in runBatchLiv.sh:

```
INDIR: local JETSCAPE repository 
CONTAINERPATH: path to JETSCAPE container
PARAMSFILE: .xml parameter file to pass to JETSCAPE
```

## skim

compress output JETSCAPE events into .parquet files

run submitSkim.sh, which bundles a number of jetscape files into a job and compresses them

some user-dependent settings required in submitSkim.sh:

```
CONTAINERPATH: path to JETSCAPE container (note - should use one which includes all required packages including heppy - leave as /bundle/data/ALICE/users/jnorman/containers/local/stat_local_gcc_v5.3.sif)
ANALYSIS_PATH: path of JETSCAPE-analysis
BASE_DIR=: path containing final_state_hadron files, i.e. the output of the generation step
N_EVENTS_PER_FILE: number of events per parquet file
N_FILES_PER_JOB: number of files to skim per job
```
