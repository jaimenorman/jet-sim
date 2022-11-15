#!/bin/bash

basedirin='/hepstore/jnorman/JEWEL/20221006.1054' # directory containing your hepMC files
cores=16  # or 40, or whatever - number of jobs to run in parallel
basedirout='/hepstore/jnorman/JEWEL/20221006.1054/analysis' # directory to output the files to
outname='output' # name to give your output root file - saved in

# check output directory exist
if [ ! -d "$basedirout" ]; then
    echo "output dir $basedirout does not exist."
    exit 1
fi

# check output directory exist
if [ ! -d "$basedirout" ]; then
    echo "input dir $basedirin does not exist."
    exit 1
fi


# loop over the files
for i in {1..20}
do
    # echo ./analyze_hepmc2_hjet $basedirin/jewel-out_$i.hepmc $basedirout/$outname_$i --chargedjets & # you can uncomment this line and comment the line below to test that the code is doing what you want!
    ./analyze_hepmc2_hjet $basedirin/jewel-out_$i.hepmc $basedirout/$outname_$i --chargedjets & # run compiled code - ampersand at end allows to continue on a new thread

    # Check how many background jobs there are, and if it
    # is equal to the number of cores, wait for anyone to
    # finish before continuing.
    while :; do
        background=( $(jobs -p))
        if (( ${#background[@]} < cores )); then
            break
        fi
        sleep 1
    done
done
