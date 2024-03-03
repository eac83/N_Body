# N_Body
N-body gravity solver.

# Compilation
Move into `src` and run
`g++ main.cc star.cc utils.cc -o main`

# Usage
## Parameter file
`parameters.txt`
* `deltaTime`: time step (s)
* `maxTime`: time code runs until (s)
* `stepsBetweenSnapshots`: number of timesteps between saved snapshots.
Provided parameter file steps one day for a total of one Jupiter year, saving snapshots every 10 days.

## Initial conditions
`initial_conditions.csv`
Each row contains the attributes for each star. The columns are:
* `m` mass
* `x` x-coordinate
* `y` y-coordinate
* `z` z-coordinate
* `vx` velocity along x-axis
* `vy` velocity along y-axis
* `vz` velocity along z-axis
* `ax` acceleration along x-axis
* `ay` acceleration along y-axis
* `az` acceleration along z-axis
Provided initial conditions represent the Sun, Earth, Mars, and Jupiter.

# Analysis
Example Python scripts for visualization and analysis in `python` directory.

## `plotting.py`
Plots the Sun, Earth, Mars, Jupiter system for one Jupiter year.

## `earth_distance.py`
Plots the distance of Earth from the origin.
