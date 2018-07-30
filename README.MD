# TNT: Tensor-based Triple Store

*work in progress.*

TNT is a SPARQL triple store to query RDF data for the Semantic Web. 
It is based on tensors and tensor algebra. Currently, it supports only SELECT with or without DISTINCT and a WHERE-bock with triple patterns only.  

## usage
### start
after building run
```
tnt_exec -p 8090 -p my_nt_file.nt
```
to load the data from the provided `.nt` file and serve SPARQL endpoint at port 8090.

### query
The endpoint may be queried locally at: `127.0.0.1:8090/sparql?query=*your query*`. 

*Notice*: the query string `*your query*` must be URL encoded. You can use any online URL encoder like <https://meyerweb.com/eric/tools/dencoder>.   

### usage example

Consider the query below against a small SP²Bench data set:
``` 
PREFIX rdf:   <http://www.w3.org/1999/02/22-rdf-syntax-ns#>
PREFIX bench: <http://localhost/vocabulary/bench/>

SELECT DISTINCT ?article
WHERE {
  ?article rdf:type bench:Article .
  ?article ?property ?value 
}
```

To run it start TNT with: 
```
tnt_exec -p 3030 -f tests/dataset/sp2b.nt 
```

Then visit with a browser of your choice the IRI to see the resul:

<http://127.0.0.1/sparql?query=PREFIX%20rdf%3A%20%20%20%3Chttp%3A%2F%2Fwww.w3.org%2F1999%2F02%2F22-rdf-syntax-ns%23%3E%0APREFIX%20bench%3A%20%3Chttp%3A%2F%2Flocalhost%2Fvocabulary%2Fbench%2F%3E%0A%0ASELECT%20DISTINCT%20%3Farticle%0AWHERE%20%7B%0A%20%20%3Farticle%20rdf%3Atype%20bench%3AArticle%20.%0A%20%20%3Farticle%20%3Fproperty%20%3Fvalue%20%0A%7D>


## prerequisites

### build tools

Compiling is currently only tested on Ubuntu 18.04 with g++-7 and some g++-8 headers.

To compile install the following build tools:
```
sodo apt install build-essential g++7 g++8 cmake
```

To run tests you need to additionally install gtest:
```
sudo apt install libgtest-dev
```
 
and compile it:
```
cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libg* /usr/lib/ && cd -
``` 

### dependencies


Clone, build and install pistache. Therefore, see below or at <https://github.com/oktal/pistache>.

To download the latest available release, clone the repository over github.
```
git clone https://github.com/oktal/pistache.git
```
Then, init the submodules:
```
git submodule update --init
```
Now, compile the sources:

```
cd pistache
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

 


antlr4 is downloaded and compiled automatically during the build but requires uuid:

```
sudo apt install uuid-dev
```

For parsing `.nt` files libserd is required:
```
sudo apt install libserd-dev
```

Further, boost and boost-context is required:
```
sudo apt install libboost-dev libboost-context-dev
```



## build

run `cmake CMakeLists.txt && make` in this directory.
