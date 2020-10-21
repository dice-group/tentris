FROM ubuntu:focal AS builder
ARG DEBIAN_FRONTEND=noninteractive
ARG CXXFLAGS="${CXXFLAGS} -march=x86-64"
ARG CMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld-10"
ARG TENTRIS_MARCH="x86-64"

RUN apt-get -qq update && \
    apt-get -qq install -y make cmake uuid-dev git openjdk-11-jdk python3-pip python3-setuptools python3-wheel libstdc++-10-dev clang-10 g++-10 pkg-config google-perftools libgoogle-perftools-dev
# we need serd as static library. Not available from ubuntu repos
RUN ln -s /usr/bin/python3 /usr/bin/python && \
    git clone --quiet --branch v0.30.2 https://gitlab.com/drobilla/serd.git && \
    cd serd && \
    git submodule update --quiet --init --recursive && \
    ./waf configure --static && \
    ./waf install &&\
    rm /usr/bin/python

# install and configure conan
RUN pip3 install conan && \
    conan user && \
    conan profile new --detect default && \
    conan profile update settings.sparql-parser-base:compiler.version=10 default && \
    conan profile update settings.sparql-parser-base:compiler.libcxx=libstdc++11  default  && \
    conan profile update settings.sparql-parser-base:compiler=gcc default &&\
    conan profile update env.sparql-parser-base:CXX=/usr/bin/g++-10 default && \
    conan profile update env.sparql-parser-base:CC=/usr/bin/gcc-10 default && \
    conan profile update settings.compiler=clang default &&\
    conan profile update settings.compiler.version=10 default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default && \
    conan profile update env.CXX=/usr/bin/clang++-10 default && \
    conan profile update env.CC=/usr/bin/clang-10 default

# add conan repositories
RUN conan remote add tsl https://api.bintray.com/conan/tessil/tsl
RUN conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
RUN conan remote add stiffstream https://api.bintray.com/conan/stiffstream/public
RUN conan remote add dice-group https://api.bintray.com/conan/dice-group/tentris

# build and cache dependencies via conan
COPY conanfile.txt /conan_cache/conanfile.txt
RUN cd /conan_cache && conan install . --build=missing --profile default > conan_build.log

# import project files
COPY thirdparty /tentris/thirdparty/
COPY src /tentris/src/
COPY CMakeLists.txt /tentris/CMakeLists.txt
COPY conanfile.txt /tentris/conanfile.txt

##build
# import and build depenedencies via conan
RUN mkdir /tentris/build && cd /tentris/build && \
    conan install .. --build=missing
# build tentris_server with clang++-10 again
RUN export CXX="clang++-10" && export CC="clang-10" && \
    cd /tentris/build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DTENTRIS_BUILD_WITH_TCMALLOC=true -DTENTRIS_STATIC=true .. && \
    make -j $(nproc)

FROM scratch
WORKDIR /tentris
COPY --from=builder /tentris/build/tentris_server /tentris_server
COPY --from=builder /tentris/build/tentris_terminal /tentris_terminal
COPY --from=builder /tentris/build/ids2hypertrie /ids2hypertrie
COPY --from=builder /tentris/build/rdf2ids /rdf2ids
ENTRYPOINT ["/tentris_server"]
