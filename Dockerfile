FROM ubuntu:eoan AS builder

RUN apt-get update && \
    apt-get install -y make cmake uuid-dev git openjdk-8-jdk python3-pip python3-setuptools python3-wheel libstdc++-9-dev g++-9 gcc-9 pkg-config
RUN pip3 install conan

ARG CXX=g++-9
ARG CC=gcc-9

RUN mkdir /conan_cache
RUN conan user && \
    conan profile new --detect default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default
RUN conan remote add tsl https://api.bintray.com/conan/tessil/tsl && \
    conan remote add public-conan https://api.bintray.com/conan/bincrafters/public-conan && \
    conan remote add stiffstream https://api.bintray.com/conan/stiffstream/public && \
    conan remote add dice-research https://api.bintray.com/conan/dice-group/tentris

COPY conanfile.txt /conan_cache/conanfile.txt
RUN cd /conan_cache && conan install . --build=missing

COPY cmake /tentris/cmake/
COPY thirdparty /tentris/thirdparty/
COPY Sparql.g4 /tentris/Sparql.g4
COPY src /tentris/src/
COPY CMakeLists.txt /tentris/CMakeLists.txt
COPY conanfile.txt /tentris/conanfile.txt

RUN mkdir /tentris/build && \
    cd /tentris/build && conan install .. --build=missing && \
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. && \
    make -j $(nproc) tentris_server

FROM ubuntu:eoan
RUN apt-get update && \
    apt-get install -y uuid
WORKDIR /tentris
COPY --from=builder /tentris/build/tentris_server /tentris_server
ENTRYPOINT ["/tentris_server"]
