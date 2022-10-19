# h+jet code and submission tools to run on HepMC events

for use on Liverpool HEP systems. 

## setup

we need some software packages (notably HepMC and fastjet). The easiest way to set these up is 
through the CERN cvmfs software distribution service ( https://cernvm.cern.ch/fs/ ):

```
# use ALICE software packages from cvmfs
source /cvmfs/alice.cern.ch/etc/login.sh 
#  load ALICE software (for fast jet) and HepMC  
/cvmfs/alice.cern.ch/bin/alienv enter VO_ALICE@AliPhysics::vAN-20200108_ROOT6-1,VO_ALICE@HepMC::HEPMC_02_06_10-1
```

## running the code

We can simply compile the code, using the provided Makefile:

```
make analyze_hepmc2_hjet
```

This will create a compiled executable file
To run this executable on a HepMC file:

```
./analyze_hepmc2_hjet /path/to/hepmc/file/jewel-out_1.hepmc /path/to/new/output/file/output
```

This creates a root file containing histograms, etc


TODO: add submission scripts for submission of batch jobs via slurm

