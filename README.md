# sparse_solver_builds

Turn-key containerized builds for high-performance sparse solvers: **MUMPS 5.8.0** and **SuiteSparse with CUDA support**. This repository provides clean, reproducible environments using both **Docker** and **Singularity**, including:

- Full MEX compilation support for MATLAB (tested with R2025a trial)
- CUDA-enabled CHOLMOD from SuiteSparse
- Support for 64-bit integer MUMPS builds using `-DINTSIZE64`
- Slurm/HPC-ready `.sif` Singularity images
- MATLAB-compatible MUMPS `.mexa64` builds with OpenMP
- Notes on known CUDA driver/pinned memory issues and how to mitigate them

---

## System Requirements and Tested Versions

| Component      | Version/Tested With             |
|----------------|---------------------------------|
| OS             | Ubuntu 24.04 LTS                |
| Docker         | 27.3.1                          |
| Singularity-CE | 4.2.1                           |
| MATLAB         | R2025a Trial (2FA login GUI)    |
| NVidia Driver  | 570.169                         |
| CUDA Toolkit   | 12.9 (display GPU)              |
| GPU            | NVIDIA RTX 3090 (compute 8.6)   |
| MUMPS          | 5.8.0  (May 2025,newest)        |
| SuiteSparse    | 7.10.3 (May 2025,newest)        |

> This repo assumes you have a working NVIDIA driver and CUDA runtime on your host. All builds are performed assuming a **GPU with compute capability 8.6**.

---

## Quick Start

### Build MUMPS
```bash
cd docker/mumps_580/
docker build -t mumps-dev .
```

### Compile MATLAB `.mexa64` from Singularity Sandbox
```bash
singularity build --sandbox mumps-dev/ docker-daemon://mumps-dev:latest
singularity shell -B /path/to/matlab:/matlab mumps-dev/
# inside: cd /src && make all
```

### Package the Slim HPC Runtime
```bash
sudo singularity build mumps_slim_final.sif mumps_slim.def
```

---

### Test SuiteSparse with GPU
```bash
cd docker/suitesparse/
docker build -t suitesparse-cuda .
docker run --rm -it suitesparse-cuda /bin/bash
CHOLMOD_USE_GPU=1 /usr/src/SuiteSparse/CHOLMOD/Demo/cholmod_dl_demo < ~/nd12k.mtx
```

---

## Repo Layout

```
sparse_solver_builds/
├── docker/
│   ├── mumps_580/         # MUMPS 5.8.0 with MATLAB + Docker
│   └── suitesparse/       # SuiteSparse + CUDA
├── singularity/
│   └── mumps/             # Slim .sif image for HPC deployment
└── README.md              # You're here!
```

Each subdirectory has its own README with details on how to build, test, and deploy.

---

## Author

Gordon Koerner, 2025  
Experimental Computing | HPC | Sparse Solvers
