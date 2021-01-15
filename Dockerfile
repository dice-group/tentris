FROM ubuntu:groovy AS builder
ARG DEBIAN_FRONTEND=noninteractive
ARG CXXFLAGS="${CXXFLAGS} -march=x86-64"
ARG CMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld-11"
ARG TENTRIS_MARCH="x86-64"

RUN apt-get -qq update && \
    apt-get -qq install -y make cmake uuid-dev git openjdk-11-jdk python3-pip python3-setuptools python3-wheel libstdc++-10-dev clang-11 g++-10 pkg-config google-perftools libgoogle-perftools-dev

ARG CXX="clang++-11"
ARG CC="clang-11"
# we need serd as static library. Not available from ubuntu repos
RUN ln -s /usr/bin/python3 /usr/bin/python
RUN git clone --quiet --branch v0.30.2 https://gitlab.com/drobilla/serd.git
WORKDIR serd
RUN git submodule update --quiet --init --recursive && \
    ./waf configure --static && \
    ./waf install
RUN rm /usr/bin/python
WORKDIR /

# install and configure conan
RUN pip3 install conan && \
    conan user && \
    conan profile new --detect default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default

# add conan repositories
RUN conan remote add tsl https://api.bintray.com/conan/tessil/tsl
RUN conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
RUN conan remote add stiffstream https://api.bintray.com/conan/stiffstream/public
RUN conan remote add dice-group https://api.bintray.com/conan/dice-group/tentris

# build and cache dependencies via conan
WORKDIR /conan_cache
COPY conanfile.txt conanfile.txt
RUN conan install . --build=missing --profile default > conan_build.log

# import project files
WORKDIR /tentris
COPY thirdparty thirdparty
COPY src src
COPY CMakeLists.txt CMakeLists.txt
COPY conanfile.txt conanfile.txt

##build
WORKDIR /tentris/build
RUN conan install .. --build=missing
RUN cmake -DCMAKE_BUILD_TYPE=Release -DTENTRIS_BUILD_WITH_TCMALLOC=true -DTENTRIS_STATIC=true ..
RUN make -j $(nproc)

FROM scratch
WORKDIR /tentris
COPY --from=builder /tentris/build/tentris_server /tentris_server
COPY --from=builder /tentris/build/tentris_terminal /tentris_terminal
COPY --from=builder /tentris/build/ids2hypertrie /ids2hypertrie
COPY --from=builder /tentris/build/rdf2ids /rdf2ids
COPY LICENSE LICENSE
COPY README.MD README.MD
ENTRYPOINT ["/tentris_server"]
