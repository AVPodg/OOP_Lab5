FROM gcc:14

RUN apt-get update && \
    apt-get install -y \
        cmake \
        git \
        gdb \
        valgrind \
        libgtest-dev \
        && rm -rf /var/lib/apt/lists/*

RUN cd /usr/src/gtest && \
    cmake CMakeLists.txt && \
    make && \
    cp lib/*.a /usr/lib && \
    mkdir -p /usr/local/lib/googletest && \
    cp -r include /usr/local/include

WORKDIR /workspace