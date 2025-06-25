# Start with minimal ubuntu 24.04 image
FROM ubuntu:24.04

# Set the working directory
WORKDIR /usr/src

# Set environment variables to avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive
ENV LD_LIBRARY_PATH=/usr/local/lib

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
RUN wget https://github.com/Kitware/CMake/releases/download/v3.26.4/cmake-3.26.4.tar.gz && \
    tar -zxvf cmake-3.26.4.tar.gz && \
    cd cmake-3.26.4 && \
    ./bootstrap && \
    make -j$(nproc) && \
    make install && \
    cd .. && rm -rf cmake-3.26.4*

# Install basic utilities
RUN apt-get update && \
    apt-get install -y libnsl-dev \
                       libgdbm-dev \
                       libgdbm-compat-dev \
                       libbz2-dev \
                       libreadline-dev \
                       libffi-dev \
                       libsqlite3-dev \
                       zlib1g-dev \
                       libncursesw5-dev \
                       libgdbm-dev \
                       liblzma-dev \
                       autoconf \
                       automake \
                       ccache \
                       tk-dev \
                       libgmp-dev \
                       libmpfr-dev \
                       openjdk-11-jdk \
                       zip \
                       unzip && \
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
    git checkout develop && \
    make NO_SHARED=0 DYNAMIC_ARCH=1 BINARY=64 INTERFACE64=1 CC=gcc FC=gfortran USE_OPENMP=1 CFLAGS="-fPIC" FFLAGS="-fPIC" NUM_THREADS=1024 && \
    make NO_SHARED=0 DYNAMIC_ARCH=1 BINARY=64 INTERFACE64=1 CC=gcc FC=gfortran USE_OPENMP=1 CFLAGS="-fPIC" FFLAGS="-fPIC" NUM_THREADS=1024 PREFIX=/usr/local install && \
    cd /usr/src && rm -rf OpenBLAS

# Create symlinks for the developer OpenBLAS static library to libblas.a and liblapack.a
RUN ln -s /usr/local/lib/libopenblasp*.a /usr/local/lib/libblas.a && \
    ln -s /usr/local/lib/libopenblasp*.a /usr/local/lib/liblapack.a

# Clone, build, and install GKlib with -fPIC
RUN git clone https://github.com/KarypisLab/GKlib.git && \
    cd GKlib && \
    make config prefix=/usr/local CFLAGS='-fPIC' openmp=set && \
    make -j$(nproc) CFLAGS='-fPIC' openmp=set && \
    make install && \
    cd .. && rm -rf GKlib

# Clone, build, and install METIS with -fPIC
RUN git clone https://github.com/KarypisLab/METIS.git && \
    cd METIS/include && \
    sed -i 's/#define IDXTYPEWIDTH 32/#define IDXTYPEWIDTH 64/' metis.h && \
    cd .. && \
    make config prefix=/usr/local CFLAGS='-fPIC' openmp=set gklib_path=/usr/local/lib && \
    make -j$(nproc) CFLAGS='-fPIC' openmp=set gklib_path=/usr/local/lib && \
    make install && \
    cd .. && rm -rf METIS

# Unpack MUMPS and copy the Makefile.inc over
COPY MUMPS_5.8.0.tar.gz /usr/src
RUN tar zxvf /usr/src/MUMPS_5.8.0.tar.gz -C /usr/src/ && \
    rm -r MUMPS_5.8.0.tar.gz
COPY Makefile.inc /usr/src/MUMPS_5.8.0
COPY make.inc /usr/src/MUMPS_5.8.0/MATLAB
COPY mumps_int_def.h /usr/src/MUMPS_5.8.0/include/

# Fix broken int(...,4)) and int(...,4)
RUN find /usr/src/MUMPS_5.8.0/src -name '*.F' -exec sed -i -E \
  -e 's/CALL omp_set_num_threads\(int\(([^,]+),4\)\)\)/CALL omp_set_num_threads(\1)/g' \
  -e 's/CALL omp_set_num_threads\(int\(([^,]+),4\)\)/CALL omp_set_num_threads(\1)/g' {} +

# Remove trailing extra ')' if present after omp_set_num_threads(...)
RUN find /usr/src/MUMPS_5.8.0/src -name '*.F' \
  -exec sed -i -E 's/(CALL +omp_set_num_threads\([^()]+\))\)/\1/g' {} +

# Fix USE OMP_LIB
RUN find /usr/src/MUMPS_5.8.0/src -name '*.F' -exec sed -i \
  's/^![$] *USE OMP_LIB/      USE OMP_LIB/' {} +

# Force ILP64 header (int64_t for MUMPS_INT)
RUN cp /usr/src/MUMPS_5.8.0/src/mumps_int_def64_h.in /usr/src/MUMPS_5.8.0/include/mumps_int_def.h

# ammend matlab wrapper to include support for 64 bit ints
RUN sed -i '1i#include <stdint.h>\n#ifdef INTSIZE64\ntypedef int64_t mumps_int;\n#else\ntypedef int32_t mumps_int;\n#endif' /usr/src/MUMPS_5.8.0/MATLAB/mumpsmex.c
    
# Replace `int *` with `mumps_int *` where appropriate
# Safe blanket replacement (quick + dirty)
RUN sed -i -E 's/,( *[^,]+ *,) *int *\)/,\1 mumps_int)/g' /usr/src/MUMPS_5.8.0/MATLAB/mumpsmex.c
# Replace some more instances with targetted code (stubborn lines didn't change)
RUN sed -i 's/EXTRACT_FROM_MATLAB_TOPTR(PERM_IN,\(.*\),int,/EXTRACT_FROM_MATLAB_TOPTR(PERM_IN,\1,mumps_int,/' /usr/src/MUMPS_5.8.0/MATLAB/mumpsmex.c && \
    sed -i 's/EXTRACT_FROM_MATLAB_TOPTR(VAR_SCHUR,\(.*\),int,/EXTRACT_FROM_MATLAB_TOPTR(VAR_SCHUR,\1,mumps_int,/' /usr/src/MUMPS_5.8.0/MATLAB/mumpsmex.c

# Set BUILDDIR environment variable
ENV BUILDDIR=/usr/src/MUMPS_5.8.0

# Set environment variables for MUMPS paths
ENV MUMPS_DIR=/usr/src/MUMPS_5.8.0
ENV PATH=$MUMPS_DIR/bin:$PATH
ENV LD_LIBRARY_PATH=$MUMPS_DIR/lib:$LD_LIBRARY_PATH
ENV LIBRARY_PATH=$MUMPS_DIR/lib:$LIBRARY_PATH
ENV CPATH=$MUMPS_DIR/include:$MUMPS_DIR/PORD/include:$CPATH

# Install MUMPS
RUN cd /usr/src/MUMPS_5.8.0 && \
    make all

# Reset DEBIAN_FRONTEND to default
ENV DEBIAN_FRONTEND=dialog

# Default command
CMD ["bash"]
