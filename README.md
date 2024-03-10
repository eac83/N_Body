# N_Body
A multithreaded C++ N-body gravity solver.

# Compilation
Move into `src` and run

## N-body solver
`g++ main.cc star.cc utils.cc -o main`

## Initial conditions

### Random
`g++ random.cc star.cc utils.cc -o random`

### Lattice

`g++ lattice.cc star.cc utils.cc -o lattice`

# Usage
## Parameter file
`parameters.txt`
* `deltaTime`: time step (s)
* `maxTime`: time code runs until (s)
* `stepsBetweenSnapshots`: number of timesteps between saved snapshots.
* `boxSize`: length of the periodic box (m)
* `softeningLength`: gravitational softening length (m)

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

# Analysis
Example Python scripts for visualization and analysis in `python` directory.

## `plotting.py`
Creates an animation of the Sun, Earth, Mars, Jupiter system for one Jupiter year.

## `earth_distance.py`
Plots the distance of Earth from the origin.

## `3d_plotting.py`
Creates an animation of the evolution of the box in 3d.

## `2d_plotting.py`
Creates an animation of the evolution of the box in the *x*-*y* plane.
