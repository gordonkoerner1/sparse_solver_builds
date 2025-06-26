# SuiteSparse Solver Docker Image

This container provides a full build of SuiteSparse with GPU support, including CHOLMOD with CUDA. You can run GPU-enabled demos and tests directly from within the container.

## Usage Instructions

1. **Build the Docker Image**  
   From the `docker/suitesparse/` directory, run:

   ```bash
   docker build -t suitesparse-dev .
   ```

2. **Run a Container and Test Examples**

   ```bash
   docker run -it -gpus=all suitesparse-dev
   ```
   inside the container, test CHOLMOD with GPU enabled (the Dockerfile provides 4 test matrices from the sparse matrix repository; they're all sparse symmetric positive-definite
   ```bash
   CHOLMOD_USE_GPU=1 /usr/src/SuiteSparse-7.0.0/build/CHOLMOD/Demo/cholmod_dl_demo < ~/nd12k.mtx
   ```

3. **Observe the pinned memory bloat/bug**

   each time I ran this, about 10GB of my system memory would be unusable and pinned for the GPU operation

These was the end of my test, I did not choose to mexa-compile suitesparse or extend it to the slurm HPC for testing: there seems to be an issue with cuda-enabled CHOLMOD
