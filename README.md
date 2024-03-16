# N_Body
A multithreaded C++ N-body gravity solver.

# Compilation
Move into `N_Body` and run `make`. Executables are then found in `/bin`:
* `main` - _N_-body code.
* `random` - Initializes stars randomly throughout the box.
* `lattice` - Initializes stars in a cubic lattice. throughout the box.

# Usage
## `main`
Usage: `main` [OPTIONS]
Options:
    `--threads`, `-t` `<num>`        Number of threads to use (default: maximum available threads)
    `--parameters`, `-p` `<file>`    Path to parameter file (default: N_Body/parameters.txt)
    `--initials`, `-i` `<file>`      Path to initial conditions file (default: N_Body/initial_conditions.csv)
    `--help`, `-h`                   Display this help message and exit

## Parameter file
`parameters.txt`
* `deltaTime`: time step (s).
* `maxTime`: time code runs until (s).
* `stepsBetweenSnapshots`: number of timesteps between saved snapshots.
* `boxSize`: size of the periodic box.
* `softeningLength`: gravitational softening length.

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

## `3d_plotting.py`
Creates an animation of the box in 3*d*.

## `2d_plotting.py`
Creates an animation of the box in 2*d*, in the *x*-*y* plane.

## `plotting.py`
Creates an animation of the Sun, Earth, Mars, Jupiter system for one Jupiter year.

## `earth_distance.py`
Plots the distance of Earth from the origin.