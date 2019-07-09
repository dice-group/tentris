FROM ubuntu:18.10
RUN apt-get update && apt-get install -y build-essential cmake uuid-dev libserd-dev g++ git openjdk-8-jdk python3-pip

RUN pip3 install conan
RUN mkdir /conan_cache
COPY conanfile.txt /conan_cache/conanfile.txt
RUN conan remote add tsl https://api.bintray.com/conan/tessil/tsl && conan remote add public-conan https://api.bintray.com/conan/bincrafters/public-conan && conan remote add stiffstream https://api.bintray.com/conan/stiffstream/public
RUN cd /conan_cache && conan install . --build=missing

COPY cmake /tentris/cmake/
COPY thirdparty /tentris/thirdparty/
COPY Sparql.g4 /tentris/Sparql.g4
COPY src /tentris/src/
COPY CMakeLists.txt /tentris/CMakeLists.txt
COPY conanfile.txt /tentris/conanfile.txt

RUN mkdir /tentris/build && cd /tentris/build
RUN cd /tentris/build && conan install .. --build=missing
RUN cd /tentris/build && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. && make -j tentris_server
ENTRYPOINT ["/tentris/build/tentris_server"]
