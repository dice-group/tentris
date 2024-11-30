FROM alpine:3.20 AS builder
ARG MARCH="x86-64-v3"

RUN apk update && \
    apk add \
    make cmake autoconf automake pkgconfig \
    gcc g++ gdb \
    clang17 clang17-dev clang17-libs clang17-extra-tools clang17-static lldb llvm17 llvm17-dev lld \
    openjdk11-jdk \
    pythonispython3 py3-pip \
    bash git libtool util-linux-dev linux-headers patch pipx


ENV CC="/usr/bin/clang-17"
ENV CXX="/usr/bin/clang++-17"
ENV CXXFLAGS="${CXXFLAGS} -march=${MARCH}"
ENV CONAN_DISABLE_STRICT_MODE=1
ENV PIPX_BIN_DIR="/usr/local/bin"

# use lld as default linker
RUN rm /usr/bin/ld && ln -s /usr/bin/lld /usr/bin/ld

# Compile more recent tcmalloc-minimal
WORKDIR /usr/local/src
RUN git clone --quiet --branch gperftools-2.15 --depth 1 https://github.com/gperftools/gperftools
WORKDIR /usr/local/src/gperftools
RUN ./autogen.sh
RUN ./configure \
    --enable-minimal \
    --disable-debugalloc \
    --enable-sized-delete \
    --enable-dynamic-sized-delete-support && \
    make -j$(nproc) && \
    make install

# install and configure conan
RUN pipx install conan==2.9.3 && \
    conan profile detect && \
    echo '[settings]'                                                                     > ~/.conan2/profiles/default && \
    echo 'os={{ detect_api.detect_os() }}'                                               >> ~/.conan2/profiles/default && \
    echo 'arch={{ detect_api.detect_arch() }}'                                           >> ~/.conan2/profiles/default && \
    echo 'build_type=Release'                                                            >> ~/.conan2/profiles/default && \
    echo 'compiler=clang'                                                                >> ~/.conan2/profiles/default && \
    echo 'compiler.version=17'                                                           >> ~/.conan2/profiles/default && \
    echo 'compiler.cppstd=20'                                                            >> ~/.conan2/profiles/default && \
    echo 'compiler.libcxx=libstdc++11'                                                   >> ~/.conan2/profiles/default

# add conan repositories
RUN conan remote add dice-group https://conan.dice-research.org/artifactory/api/conan/tentris

# import project files
WORKDIR /usr/local/src/tentris
COPY libs libs
COPY execs execs
COPY cmake cmake
COPY CMakeLists.txt .
COPY conanfile.py .
RUN wget https://github.com/conan-io/cmake-conan/raw/develop2/conan_provider.cmake -O conan_provider.cmake

# build
WORKDIR /usr/local/src/tentris/build
RUN cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DWITH_TCMALLOC=true \
    -DSTATIC=true \
    -DMARCH=${MARCH} \
    -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake" \
    ..
RUN make -j $(nproc)

FROM scratch AS binaries
COPY --from=builder /usr/local/src/tentris/build/execs/tentris-server/tentris_server /tentris_server
COPY --from=builder /usr/local/src/tentris/build/execs/tentris-loader/tentris_loader /tentris_loader
COPY --from=builder /usr/local/src/tentris/build/execs/tools/deduplicated-nt/deduplicated_nt /deduplicated_nt
COPY --from=builder /usr/local/src/tentris/build/execs/tools/rdf2ids/rdf2ids /rdf2ids
COPY README.MD README.MD
ENTRYPOINT ["/tentris_server"]