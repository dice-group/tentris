# Tentris: Tensor-based Triple Store

[![Build Status](https://travis-ci.com/dice-group/tnt.svg?branch=development)](https://travis-ci.com/dice-group/tnt)

<span style="font-variant:small-caps;">Tentris</span> is a triple store to query RDF data using SPARQL for the Semantic Web. 
It is based on tensors and tensor algebra. 
Currently, it supports SELECT queries with or without DISTINCT and a WHERE-bock with triple patterns.

Further SPARQL features are to follow.   

## running tentris 
Tentris provides two ways of running it. Either as a HTTP endpoint or as a interactive commandline tool. 
HTTP endpoint.
### HTTP endpoint
#### start
To start Tentris as a HTTP endpoint start it with
after building run
```
tentris_server -p 8090 -f my_nt_file.nt
```
to load the data from the provided `.nt` file and serve SPARQL endpoint at port 8090. 
For more options commandline options see ` tentris_server --help`. 

#### query
The endpoint may now be queried locally at: `127.0.0.1:8090/sparql?query=*your query*`. 

*Notice*: the query string `*your query*` must be URL encoded. 
You can use any online URL encoder like <https://meyerweb.com/eric/tools/dencoder>.   

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

For more commandline options see `tentris_terminal`.

## prerequisites

### build tools

Tentris is known to build successfully on Ubuntu 18.04 and newer. 
Building was tested with GCC 8 and clang 7.

The following packages are required to build tetnris:
```
sudo apt install build-essential cmake uuid-dev libserd-dev libboost-all-dev g++ git openjdk-8-jdk
```

### dependencies

Additionally, [pistache](https://github.com/oktal/pistache) needs to be compiled and installed on your system.
But don't worry. It is really easy. Just follow the steps below.

To download the latest available release, clone the repository from github:
```
git clone https://github.com/oktal/pistache.git
```
Then, init the submodules:
```
cd pistache
git submodule update --init
```
Now, make a build directory, go there, run cmake and compile the sources:
```
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```
## build
After you installed all dependencies, you can complile tentris now. 
Make sure you are connected to the internet as Tentris downloads things at several points throughout the build processes.

If you did not so far, clone Tentris:
```
git clone https://github.com/dice-group/tentris.git
```
Make sure you are in the cloned folder:
```
cd tentris
```
Now, make a build directory, go there, run cmake and compile the sources: 
```bash
mkdir build 
/tentris/build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. 
cd /tentris/build
make tentris_server tentris_terminal
```
Now is the time to get yourself a coffee. In about 
When you build Tentris for the first time, it will take some time. 
