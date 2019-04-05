FROM ubuntu:18.10
RUN apt-get update && apt-get install -y build-essential cmake uuid-dev libserd-dev libboost-all-dev g++ git openjdk-8-jdk
RUN git clone --depth=1 https://github.com/oktal/pistache.git && cd pistache && git submodule update --init && mkdir build && cd build && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. && make && make install
COPY cmake /tentris/cmake/
COPY thirdparty /tentris/thirdparty/
COPY Sparql.g4 /tentris/Sparql.g4
COPY src /tentris/src/
COPY CMakeLists.txt /tentris/CMakeLists.txt
RUN ls /tentris
RUN mkdir /tentris/build && cd /tentris/build && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. && cd /tentris/build && make tentris_server
ENTRYPOINT ["/tentris/build/tentris_server"]
