#!/bin/bash

# Name of the job
#SBATCH --job-name={job_id}

# Use Wojciechs account
#SBATCH --account=jarosz-lab

# Number of compute nodes
#SBATCH --nodes=1

# Number of tasks per node
#SBATCH --ntasks-per-node=1

# Number of CPUs per task
#SBATCH --cpus-per-task=1

# Request memory
#SBATCH --mem=8G

# Walltime (job duration)
#SBATCH --time=00:05:00

# Email notifications (comma-separated options: BEGIN,END,FAIL)
#SBATCH --mail-type=FAIL

#SBATCH --array=1-10

./pbrt --experiment ao_prog ${SLURM_ARRAY_TASK_ID}
