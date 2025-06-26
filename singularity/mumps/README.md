# MUMPS + MATLAB R2025a (Singularity Build)

This folder contains the Singularity sandbox build for compiling MATLAB `.mexa64` files for the MUMPS 5.8.0 sparse solver.

## Overview

We first build a Docker image with MUMPS and its dependencies. From there, we **convert the Docker image into a Singularity sandbox** so we can access the MATLAB GUI from the host (for 2FA license) and compile the `.mex` files.

---

## Docker → Singularity Sandbox

To convert the Docker image into a writable Singularity sandbox:

```bash
singularity build --sandbox mumps-dev/ docker-daemon://mumps-dev:latest
```

## Start an interactive shell in your writable sandbox
```bash
singularity shell --writable \
  -B /usr/local/MATLAB/R2025a:/opt/MATLAB/R2025a \
  -B /home/gordo/.matlab:/home/gordo/.matlab \
  mumps-dev
```

Path to the matlab directory and "make all" , "make.inc" will do all of the work.

## Leave the singularity sandbox and build a slim development singularity image file (sif) for HPC deployment, using only the compiled files we need to use mumps
```bash
sudo singularity build mumps_slim_final.sif mumps_slim.def
```

