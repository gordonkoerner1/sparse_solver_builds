# MUMPS Docker Build

This directory contains the build files for compiling **MUMPS 5.8.0** in a Docker container.
# NOTE: you'll need to request MUMPS 5.8.0 access from the developers and then download the source files:
https://mumps-solver.org/index.php?page=dwnld

## File Descriptions

| File                | Purpose |
|---------------------|---------|
| `Dockerfile`        | Docker recipe to install compilers, dependencies, and build MUMPS/SuiteSparse. |
| `Makefile.inc`      | Critical MUMPS configuration file. Controls compiler flags, paths, and parallelism (OpenMP, MPI/sequential, INT64, etc.). |
| `make.inc`          | Used to compile MATLAB MEX files with proper flags and MATLAB pathing. |
| `mumps_int_def.h`   | Defines 64-bit integer types (`-DINTSIZE64`) for compatibility with downstream tools like MATLAB and Python. |

## Build Instructions

You can test the compiled examples at /usr/src/MUMPS_5.8.0/examples, check the readme or:
```bash
./dsimpletest < input_simpletest_real
```

```bash
docker build -t mumps_build .
```

# NOTE: If you intend to use MUMPS in Matlab, you will need to get creative here and provide your own licensed Matlab, or a trial (I used a trial license for R2025a) to compile and test the mexafiles. I used a singularity sandbox on my local system for this, detailed in the singularity section of the project.
