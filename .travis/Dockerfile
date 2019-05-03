FROM ubuntu:18.10
RUN apt-get update && apt-get install -y build-essential cmake uuid-dev libserd-dev libboost-all-dev g++ git openjdk-8-jdk
RUN git clone --depth=1 https://github.com/oktal/pistache.git && cd pistache && git submodule update --init && mkdir build && cd build && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. && make -j 6&& make install
RUN cd ~
COPY . /tentris
CMD cd /tentris && cmake CMakeLists.txt && make -j 6
