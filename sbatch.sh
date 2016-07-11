#!/bin/sh

module load launcher/mic
sbatch -N 1 -p quick native_run.sh ~/GitHub/ParallelClustering/x64/Debug/ParallelClusteringPhi_x64.mic -k 3 -d 3 -n 1000 -f 1.5 -e 0.1 -i ~/GitHub/ParallelClustering/x64/Debug/input.csv -o ~/GitHub/ParallelClustering/x64/Debug/output.csv