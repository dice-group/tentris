FROM alpine:3.17 AS builder
ARG MARCH="x86-64-v3"

RUN apk update && \
    apk add \
    make cmake autoconf automake pkgconfig \
    gcc g++ gdb \
    clang15 clang15-dev clang15-libs clang15-extra-tools clang15-static lldb llvm15 llvm15-dev lld \
    openjdk11-jdk \
    pythonispython3 py3-pip \
    bash git libtool util-linux-dev linux-headers

ARG CC="clang"
ARG CXX="clang++"
ENV CXXFLAGS="${CXXFLAGS} -march=${MARCH}"
RUN rm /usr/bin/ld && ln -s /usr/bin/lld /usr/bin/ld # use lld as default linker


# Compile more recent tcmalloc-minimal with clang-14 + -march
RUN git clone --quiet --branch gperftools-2.9.1 --depth 1 https://github.com/gperftools/gperftools
WORKDIR /gperftools
RUN ./autogen.sh
RUN ./configure \
    --enable-minimal \
    --disable-debugalloc \
    --enable-sized-delete \
    --enable-dynamic-sized-delete-support && \
    make -j$(nproc) && \
    make install
WORKDIR /

ENV CONAN_DISABLE_STRICT_MODE=1

# install and configure conan
RUN pip3 install conan==1.62.0 && \
    conan user && \
    conan profile new --detect default && \
    conan profile update settings.compiler=clang default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default && \
    conan profile update settings.compiler.cppstd=20 default && \
    conan profile update env.CXXFLAGS="${CXXFLAGS}" default && \
    conan profile update env.CXX="${CXX}" default && \
    conan profile update env.CC="${CC}" default && \
    conan profile update options.boost:extra_b2_flags="cxxflags=\\\"${CXXFLAGS}\\\"" default && \
    conan profile update options.boost:header_only=True default && \
    conan profile update options.restinio:asio=boost default

# add conan repositories
RUN conan remote add dice-group https://conan.dice-research.org/artifactory/api/conan/tentris

# build and cache dependencies via conan
WORKDIR /conan_cache
COPY conanfile.py .
COPY CMakeLists.txt .
RUN conan install . --build=* --profile default
# import project files
WORKDIR /tentris
COPY libs libs
COPY execs execs
COPY cmake cmake
COPY CMakeLists.txt .
COPY conanfile.py .

##build
WORKDIR /tentris/execs/build
RUN cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DWITH_TCMALLOC=true \
    -DSTATIC=true \
    -DMARCH=${MARCH} \
    ..
RUN make -j $(nproc)

FROM scratch
COPY --from=builder /tentris/execs/build/tentris-server/tentris_server /tentris_server
COPY --from=builder /tentris/execs/build/tentris-loader/tentris_loader /tentris_loader
COPY --from=builder /tentris/execs/build/tools/deduplicated-nt/deduplicated_nt /deduplicated_nt
COPY --from=builder /tentris/execs/build/tools/rdf2ids/rdf2ids /rdf2ids
COPY README.MD README.MD
ENTRYPOINT ["/tentris_server"]
