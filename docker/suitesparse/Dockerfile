# Start with minimal ubuntu 24.04 image
FROM ubuntu:24.04

# Set the working directory
WORKDIR /usr/src

# Set environment variables to avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install basic utilities, 
RUN apt-get update && \
    apt-get install -y nano wget git make libssl-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install gcc 13, gfortran 13
RUN apt-get update && \
    apt-get install -y gcc g++ && \
    apt-get install -y gfortran && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Pin GCC to prevent its installation
RUN echo "Package: gcc*\nPin: release *\nPin-Priority: -1" > /etc/apt/preferences.d/gcc

# toolchain
ENV CC=gcc
ENV CXX=g++

# Download, build, and install CMake from source
RUN wget https://github.com/Kitware/CMake/archive/refs/tags/v4.0.2.tar.gz && \
    tar -zxvf v4.0.2.tar.gz && \
    cd CMake-4.0.2 && \
    ./bootstrap && \
    make -j$(nproc) && \
    make install && \
    cd .. && rm -rf CMake-4.0.2 && rm -r v4.0.2.tar.gz

# Install basic utilities
RUN apt-get update && \
    apt-get install -y libgdbm-dev \
                       libgdbm-compat-dev \
                       libbz2-dev \
                       libreadline-dev \
                       libffi-dev \
                       libsqlite3-dev \
                       zlib1g-dev \
                       libncursesw5-dev \
                       liblzma-dev \
                       autoconf \
                       automake \
                       ccache \
                       tk-dev \
                       libgmp-dev \
                       libmpfr-dev \
                       openjdk-11-jdk \
                       zip \
                       unzip \
                       gdb \
                       libnsl-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Clean up an empty dir
RUN rm -rf /usr/local/lib/python3.12

# Install test matrices
RUN wget https://suitesparse-collection-website.herokuapp.com/MM/ND/nd3k.tar.gz && \
    tar -xzvf nd3k.tar.gz && mv nd3k/nd3k.mtx ~/nd3k.mtx && rm -rf nd3k*
RUN wget https://suitesparse-collection-website.herokuapp.com/MM/ND/nd6k.tar.gz && \
    tar -xzvf nd6k.tar.gz && mv nd6k/nd6k.mtx ~/nd6k.mtx && rm -rf nd6k*
RUN wget https://suitesparse-collection-website.herokuapp.com/MM/ND/nd12k.tar.gz && \
    tar -xzvf nd12k.tar.gz && mv nd12k/nd12k.mtx ~/nd12k.mtx && rm -rf nd12k*
RUN wget https://suitesparse-collection-website.herokuapp.com/MM/ND/nd24k.tar.gz && \
    tar -xzvf nd24k.tar.gz && mv nd24k/nd24k.mtx ~/nd24k.mtx && rm -rf nd24k*

# Download and build OpenBLAS using make
RUN git clone https://github.com/xianyi/OpenBLAS.git && \
    cd OpenBLAS && \
    git checkout v0.3.28 && \
    make NO_SHARED=0 DYNAMIC_ARCH=1 BINARY=64 INTERFACE64=1 CC=gcc FC=gfortran USE_OPENMP=1 CFLAGS="-fPIC" FFLAGS="-fPIC" NUM_THREADS=1024 && \
    make NO_SHARED=0 DYNAMIC_ARCH=1 BINARY=64 INTERFACE64=1 CC=gcc FC=gfortran USE_OPENMP=1 CFLAGS="-fPIC" FFLAGS="-fPIC" NUM_THREADS=1024 PREFIX=/usr/local install && \
    cd /usr/src && rm -rf OpenBLAS

# Create symlinks for v0.3.28 OpenBLAS static library to libblas.a and liblapack.a
RUN ln -s /usr/local/lib/libopenblasp*.a /usr/local/lib/libblas.a && \
    ln -s /usr/local/lib/libopenblasp*.a /usr/local/lib/liblapack.a

# Copy CUDA run file to the container
COPY cuda_12.9.1_575.57.08_linux.run /usr/src/

# Install CUDA Toolkit 12.9.1
RUN sh cuda_12.9.1_575.57.08_linux.run --silent --toolkit --override && \
    rm cuda_12.9.1_575.57.08_linux.run

# Set environment variables for CUDA and toolchain
ENV PATH=/usr/local/cuda-12.9/bin:$PATH
ENV LD_LIBRARY_PATH=/usr/local/cuda-12.9/lib64
ENV LIBRARY_PATH=/usr/local/cuda-12.9/lib64
ENV CPATH=/usr/local/cuda-12.9/include

# unpack suitesparse 7.10.3, May 5 2025 release
COPY SuiteSparse-7.10.3.tar.gz /usr/src
RUN tar zxvf /usr/src/SuiteSparse-7.10.3.tar.gz -C /usr/src/ && \
    rm -r SuiteSparse-7.10.3.tar.gz

# path suitesparse
ENV LD_LIBRARY_PATH=/usr/src/SuiteSparse-7.10.3/lib:${LD_LIBRARY_PATH:-}

# build suitesparse
RUN cd /usr/src/SuiteSparse-7.10.3/build && cmake \
    -DCMAKE_EXE_LINKER_FLAGS="-L/usr/local/cuda/lib64 -lcudart -lcublas -lcusparse -lpthread -ldl -lrt -lm -Wl,--whole-archive /usr/local/lib/libopenblas.a -Wl,--no-whole-archive -lgomp -lgfortran -lm" \
    -DBLAS_LIBRARIES="/usr/local/lib/libopenblas.a" \
    -DBLAS_LINKER_FLAGS="-lgomp -lpthread -ldl -lrt -lgfortran -lm" \
    -DLAPACK_LIBRARIES="/usr/local/lib/libopenblas.a" \
    -DLAPACK_LINKER_FLAGS="-lgomp -lpthread -ldl -lrt -lgfortran -lm" \
    -DCMAKE_C_FLAGS="-I/usr/local/cuda-12.9/include -m64 -fopenmp -DCHOLMOD_GPU_VERBOSE=1 -DCHOLMOD_GPU_MEMORY_STATS=1" \
    -DCMAKE_CXX_FLAGS="-I/usr/local/cuda-12.9/include -m64 -fopenmp -DCHOLMOD_GPU_VERBOSE=1 -DCHOLMOD_GPU_MEMORY_STATS=1" \
    -DCMAKE_CUDA_FLAGS="-Xcompiler -fPIC -lineinfo -lcudart -lcublas -lcusparse -shared" \
    -DSUITESPARSE_USE_CUDA=ON \
    -DCHOLMOD_USE_CUDA=ON \
    -DSUITESPARSE_USE_64BIT_BLAS=ON \
    -DSUITESPARSE_CUDA_ARCHITECTURES="80" \
    -DSUITESPARSE_ENABLE_PROJECTS="amd;cholmod" \
    -DSUITESPARSE_DEMOS=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_CUDA_COMPILER=/usr/local/cuda-12.9/bin/nvcc \
    -DSUITESPARSE_USE_OPENMP=ON \
    .. && cmake --build . --config Release -j$(nproc)

# set cap to memory request for GPU operations in suitesparse
ENV CHOLMOD_GPU_MEM_FRACTION=0.20

# Reset DEBIAN_FRONTEND to default
ENV DEBIAN_FRONTEND=dialog

# Default command
CMD ["bash"]