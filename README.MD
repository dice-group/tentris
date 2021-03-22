# Tentris: A Tensor-based Triple Store

[![Build Status](https://travis-ci.com/dice-group/tentris.svg)](https://travis-ci.com/dice-group/tentris)

<span style="font-variant:small-caps;">Tentris</span> is a triple store to query RDF data using SPARQL. 
It is based on tensors and tensor algebra. 
Currently, it supports SELECT queries with or without DISTINCT and a WHERE-block with triple patterns.

Further SPARQL features will follow.   

## download

Precompiled linux binaries are provided under [Release](https://github.com/dice-group/tentris/releases).
Newer release binaries are fully statically linked and have no dependencies.   

Docker images of the HTTP endpoint are provided at [Docker Hub](https://hub.docker.com/r/dicegroup/tentris_server). 

## running tentris 
Tentris provides two ways of running it. Either as a HTTP endpoint or as a interactive commandline tool. Make sure you build Tentris successfully, before proceeding below.
### HTTP endpoint
#### start
To start Tentris as a HTTP endpoint run
```
tentris_server -p 8090 -f my_nt_file.nt
```
to load the data from the provided `.nt` file and serve SPARQL endpoint at port 8090. 
For more options commandline options see ` tentris_server --help`. 

#### query
The endpoint may now be queried locally at: `127.0.0.1:8090/sparql?query=*your query*`. 

*Notice*: the query string `*your query*` must be URL encoded. 
You can use any online URL encoder like <https://meyerweb.com/eric/tools/dencoder>.   

An additional endpoint is provided at `127.0.0.1:8090/stream` using chunk encoded HTTP response. This endpoint should be used for very large responses (>1mio results).


#### usage example

Consider the query below against a [SP²Bench](http://dbis.informatik.uni-freiburg.de/forschung/projekte/SP2B/) data set:
``` 
PREFIX rdf:   <http://www.w3.org/1999/02/22-rdf-syntax-ns#>
PREFIX bench: <http://localhost/vocabulary/bench/>

SELECT DISTINCT ?article
WHERE {
  ?article rdf:type bench:Article .
  ?article ?property ?value 
}
```

To run the query start Tentris with: 
```
tentris_server -p 3030 -f sp2b.nt 
```
You can find a populated sp2b.nt file in [tests/dataset/sp2b.nt](tests/dataset/sp2b.nt).

now, visit the follwing IRI in a browser to send the query to your Tentris endpoint:

<http://127.0.0.1:3030/sparql?query=PREFIX%20rdf%3A%20%20%20%3Chttp%3A%2F%2Fwww.w3.org%2F1999%2F02%2F22-rdf-syntax-ns%23%3E%0APREFIX%20bench%3A%20%3Chttp%3A%2F%2Flocalhost%2Fvocabulary%2Fbench%2F%3E%0A%0ASELECT%20DISTINCT%20%3Farticle%0AWHERE%20%7B%0A%20%20%3Farticle%20rdf%3Atype%20bench%3AArticle%20.%0A%20%20%3Farticle%20%3Fproperty%20%3Fvalue%20%0A%7D>

## commandline tool

For small experiments it is sometimes more convenient to use a commandline tool for querying an RDF graph. 
Therefore, Tentris provides a commandline interface.  

To start Tentris as a interactive commandline tool, run:
```
tentris_terminal -f my_nt_file.nt
```

After the RDF data from `my_nt_file.nt` is loaded, you type your query and hit ENTER. After the result was printed, you can enter your next query. 

For more commandline options see `tentris_terminal --help`.

## prerequisites for compiling

### build tools

Tentris is known to build successfully on Ubuntu 18.10 and newer. 
Building was tested with GCC 9 and clang 9.

The following packages are required to build Tentris:
```
sudo apt install build-essential uuid-dev g++ git openjdk-8-jdk python3-pip python3-setuptools python3-wheel
```
Additionally, a recent version of conan is required: 
```
pip3 install --user conan
```

### dependencies
Most required dependencies are installed via conan. Therefore, Add the respective remotes:
```shell script
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan remote add tsl https://api.bintray.com/conan/tessil/tsl
conan remote add stiffstream https://api.bintray.com/conan/stiffstream/public 
conan remote add dice-group https://conan.dice-research.org/artifactory/api/conan/tentris
```
Additionally, a statically linked version of the [Serd](https://drobilla.net/software/serd) library is required. As the packages in the deb/rpm repositories include only a dynamic library, we need to compile it manually:
```shell script
git clone --branch v0.30.2 https://gitlab.com/drobilla/serd.git
cd serd
git submodule update --init --recursive
./waf configure --static
sudo ./waf install
cd -
```
## build
After you installed all dependencies, you are ready to build Tentris. 
Make sure you are connected to the internet as Tentris downloads things at several points throughout the build processes.

If you did not so far, clone Tentris:
```
git clone https://github.com/dice-group/tentris.git
```
Make sure you are in the cloned folder:
```
cd tentris
```
Now, make a build directory and enter it.
```shell script
mkdir build 
cd build
```
Get and build the dependencies with conan:
```shell script
conan install .. --build=missing --settings compiler.libcxx="libstdc++11"
```
Generate the build skripts with CMAKE and run the build:
```shell script
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. 
make -j tentris_server tentris_terminal
```
Now is the time to get yourself a coffee. In about 
When you build Tentris for the first time, it will take some time. 

The binaries will be located at `tentris/build/bin`. 

## debug and tests
To compile Tentris with debugging symbols, proceed as above but change the cmake command to `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug .. `.

To compile the tests, run `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DTENTRIS_BUILD_TESTS=True .. ` for debugging or `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DTENTRIS_BUILD_TESTS=True .. ` for release.
# docker
* A docker image is available on [docker hub](https://hub.docker.com/r/dicegroup/tentris_server). Get it with 
    ```shell script
    docker pull dicegroup/tentris_server
    ```
* To show the available commandline options, run
    ```shell script
    docker run --rm dicegroup/tentris_server --help
    ```
* Tentris uses by default the port 8090, so make sure you forward it, e.g. 
    ```shell script
    docker run --publish=9080:9080 dicegroup/tentris_server
    ```
* To load data, mount its enclosing directory to the container and tell Tentris, to load it:
    ```shell script
    docker run -v /localfolder:/datasets --publish=9080:9080 dicegroup/tentris_server -f /datasets/yourRDFfile.nt
    ```
* By default, Tentris writes logs to the `/tentris` in the container. To make logs available outside the container, you can mount them as well:
    ```shell script
    docker run -v /local-log-dir:/tentris --publish=9080:9080 dicegroup/tentris_server
    ```
* The other command-line tools `tentris_terminal`, `ids2hypertrie` and `ids2hypertrie` are also available in the container. Run them like:
    ```shell script
    docker run -it  dicegroup/tentris_server tentris_terminal
    ``` 

