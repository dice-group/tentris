FROM ubuntu:eoan AS builder

RUN apt-get update && \
    apt-get install -y make cmake uuid-dev git openjdk-8-jdk python3-pip python3-setuptools python3-wheel libstdc++-9-dev clang-9 gcc pkg-config

RUN git clone --branch v0.30.2 https://gitlab.com/drobilla/serd.git && \
    cd serd && \
    git submodule update --init --recursive && \
    ./waf configure --static && \
    ./waf install

# install and configure conan
RUN pip3 install conan && \
    conan user && \
    conan profile new --detect default && \

    conan profile update settings.sparql-parser-base:compiler.version=9 default && \
    conan profile update settings.sparql-parser-base:compiler.libcxx=libstdc++11  default  && \
    conan profile update settings.sparql-parser-base:compiler=gcc default &&\
    conan profile update env.sparql-parser-base:CXX=/usr/bin/g++ default && \
    conan profile update env.sparql-parser-base:CC=/usr/bin/gcc default && \
    conan profile update settings.compiler=clang default &&\
    conan profile update settings.compiler.libcxx=libstdc++11 default && \
    conan profile update env.CXX=/usr/bin/clang++-9 default && \
    conan profile update env.CC=/usr/bin/clang-9 default

# add conan repositories
RUN conan remote add tsl https://api.bintray.com/conan/tessil/tsl && \
    conan remote add public-conan https://api.bintray.com/conan/bincrafters/public-conan && \
    conan remote add stiffstream https://api.bintray.com/conan/stiffstream/public && \
    conan remote add dice-group https://api.bintray.com/conan/dice-group/tentris

# build and cache dependencies via conan
COPY conanfile.txt /conan_cache/conanfile.txt
RUN cd /conan_cache && conan install . --build=missing --profile default

# import project files
COPY thirdparty /tentris/thirdparty/
COPY src /tentris/src/
COPY CMakeLists.txt /tentris/CMakeLists.txt
COPY conanfile.txt /tentris/conanfile.txt

##build
# import and build depenedencies via conan
RUN mkdir /tentris/build && cd /tentris/build && \
    conan install .. --build=missing
# build tentris_server with clang++-9 again
RUN cd /tentris/build && \
    export CXX="clang++-9" && export CC="clang-9" && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j $(nproc) tentris_server

FROM ubuntu:focal
WORKDIR /tentris
COPY --from=builder /tentris/build/tentris_server /tentris_server
ENTRYPOINT ["/tentris_server"]
