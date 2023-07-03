[![Website](https://img.shields.io/website?up_message=tentris&url=https%3A%2F%2Ftentris.dice-research.org%2F)](https://tentris.dice-research.org/) [![Build Status](https://travis-ci.com/dice-group/tentris.svg)](https://travis-ci.com/dice-group/tentris) [![Docker Cloud Build Status](https://img.shields.io/docker/cloud/build/dicegroup/tentris_server)](https://hub.docker.com/repository/docker/dicegroup/tentris_server) ![GitHub](https://img.shields.io/github/license/dice-group/tentris)  

# Tᴇɴᴛʀɪs: A Tensor-based Triple Store 

<p><img src = "https://tentris.dice-research.org/iswc2020/assets/img/Tentris_logo.svg" alt = "Tᴇɴᴛʀɪs Logo" width = "30%" align = "center"></p>

Tᴇɴᴛʀɪs is a tensor-based RDF triple store with SPARQL support. It is introduced and described in:
> Alexander Bigerl, Felix Conrads, Charlotte Behning, Mohamed Ahmed Sherif, Muhammad Saleem and Axel-Cyrille Ngonga Ngomo (2020) **Tentris – A Tensor-Based Triple Store.** In: The Semantic Web – ISWC 2020

https://tentris.dice-research.org/iswc2020/

```
@InProceedings{bigerl2020tentris,
  author = {Bigerl, Alexander and Conrads, Felix and Behning, Charlotte and Sherif, Mohamed Ahmed and Saleem, Muhammad and Ngonga Ngomo, Axel-Cyrille},
  booktitle = {The Semantic Web -- ISWC 2020},
  publisher = {Springer International Publishing},
  title = { {T}entris -- {A} {T}ensor-{B}ased {T}riple {S}tore},
  pages = {56--73},
  url = {https://papers.dice-research.org/2020/ISWC_Tentris/iswc2020_tentris_public.pdf},
  year = 2020,
  isbn = {978-3-030-62419-4}
}
```

</details>

## Key Features
* fast tensor-based in-memory storage and query processing
* SPARQL Protocol conform HTTP interface
* supports at the moment SPARQL queries with SELECT + opt. DISTINCT + basic graph pattern
* GraphQL queries (see GraphQL_doc.md for more details)
* available for Linux x86-64 

<details><summary>Current limitations: </summary>

* no persistance
* SPARQL support limited to SELECT + opt. DISTINCT + basic graph pattern
* data loading only possible at startup

</details>

## Get It
* download [static prebuilt binaries](https://hobbitdata.informatik.uni-leipzig.de/tentris-graphql/tentris_server) and [try them out](#running-tentris)
* <s>pull a [docker image](https://hub.docker.com/repository/docker/dicegroup/tentris_server) and [try them out](#docker)</s> a docker image will be made available soon
* [build it yourself](#build-it-yourself)

## running Tᴇɴᴛʀɪs 
Tᴇɴᴛʀɪs provides two ways of running it. Either as a HTTP endpoint or as a interactive commandline tool. Make sure you build Tᴇɴᴛʀɪs successfully, before proceeding below.
### HTTP endpoint
<details><summary> </summary>


#### Start
To start Tᴇɴᴛʀɪs as a HTTP endpoint run
```
tentris_server -p 9080 -f my_nt_file.nt
```
to load the data from the provided `.nt` file and serve SPARQL endpoint at port 9080. 
For more options commandline options see ` tentris_server --help`. 

#### Query
The endpoint may now be queried locally at: `127.0.0.1:9080/sparql?query=*your query*`. 

*Notice*: the query string `*your query*` must be URL encoded. 
You can use any online URL encoder like <https://meyerweb.com/eric/tools/dencoder>.   

An additional endpoint is provided at `127.0.0.1:9080/stream` using chunk encoded HTTP response. This endpoint should be used for very large responses (>1mio results).

For GraphQL, we provide two additional endpoints:
  - 127.0.0.1:9080/graphqlschema, which accepts POST requests is used to define the GraphQL schema
  - 127.0.0.1:9080/graphql?query=*your query", which accepts GraphQL queries (GET requests).

#### Usage Example (SPARQL)

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

To run the query start Tᴇɴᴛʀɪs with: 
```
tentris_server -p 3030 -f sp2b.nt 
```

Now, visit the following IRI in a browser to send the query to your Tᴇɴᴛʀɪs endpoint:

<http://127.0.0.1:3030/sparql?query=PREFIX%20rdf%3A%20%20%20%3Chttp%3A%2F%2Fwww.w3.org%2F1999%2F02%2F22-rdf-syntax-ns%23%3E%0APREFIX%20bench%3A%20%3Chttp%3A%2F%2Flocalhost%2Fvocabulary%2Fbench%2F%3E%0A%0ASELECT%20DISTINCT%20%3Farticle%0AWHERE%20%7B%0A%20%20%3Farticle%20rdf%3Atype%20bench%3AArticle%20.%0A%20%20%3Farticle%20%3Fproperty%20%3Fvalue%20%0A%7D>


#### Usage Example (GraphQL)
For this example, we use an excerpt from the [SP²Bench](http://dbis.informatik.uni-freiburg.de/forschung/projekte/SP2B/) dataset (i.e., the file ./tests/data/test.nt).

We first start Tᴇɴᴛʀɪs with:

```
tentris_server -p 9080 -f ./tests/data/test.nt
```

Then, we upload a GraphQL schema capturing the data via a POST request:
```
curl -F "schema=@./tests/data/schema.graphql" 127.0.0.1:9080/graphqlschema
```

We can now issue the GraphQL query
```
{
  articles {
    title
    journal {
        number
        volume
    }
  }
}
```
as follows:
```
curl -G --data-urlencode 'query={ articles { title journal { number volume } } }' '127.0.0.1:9080/graphql'
```
</details>

### CLI Endpoint (SPARQL)

<details><summary> </summary>

For small experiments it is sometimes more convenient to use a commandline tool for querying an RDF graph. 
Therefore, Tᴇɴᴛʀɪs provides a commandline interface.  

To start Tᴇɴᴛʀɪs as a interactive commandline tool, run:
```
tentris_terminal -f my_nt_file.nt
```

After the RDF data from `my_nt_file.nt` is loaded, you type your query and hit ENTER. After the result was printed, you can enter your next query. 

For more commandline options see `tentris_terminal --help`.

</details>


[//]: # (## Docker)

[//]: # ()
[//]: # (Using the Tᴇɴᴛʀɪs docker image is really easy. Find necessary steps below.)

[//]: # ()
[//]: # (<details><summary> </summary>)

[//]: # ()
[//]: # (* A docker image is available on [docker hub]&#40;https://hub.docker.com/r/dicegroup/tentris_server&#41;. Get it with )

[//]: # (    ```shell script)

[//]: # (    docker pull dicegroup/tentris_server)

[//]: # (    ```)

[//]: # (* To show the available commandline options, run)

[//]: # (    ```shell script)

[//]: # (    docker run --rm dicegroup/tentris_server --help)

[//]: # (    ```)

[//]: # (* Tᴇɴᴛʀɪs uses by default the port 9080, so make sure you forward it, e.g. )

[//]: # (    ```shell script)

[//]: # (    docker run --publish=9080:9080 dicegroup/tentris_server)

[//]: # (    ```)

[//]: # (* To load data, mount its enclosing directory to the container and tell Tᴇɴᴛʀɪs, to load it:)

[//]: # (    ```shell script)

[//]: # (    docker run -v /localfolder:/datasets --publish=9080:9080 dicegroup/tentris_server -f /datasets/yourRDFfile.nt)

[//]: # (    ```)

[//]: # (* By default, Tᴇɴᴛʀɪs writes logs to the `/tentris` in the container. To make logs available outside the container, you can mount them as well:)

[//]: # (    ```shell script)

[//]: # (    docker run -v /local-log-dir:/tentris --publish=9080:9080 dicegroup/tentris_server)

[//]: # (    ```)

[//]: # (* The other command-line tools `tentris_terminal`, `ids2hypertrie` and `ids2hypertrie` are also available in the container. Run them like:)

[//]: # (    ```shell script)

[//]: # (    docker run -it  dicegroup/tentris_server tentris_terminal)

[//]: # (    ```)

[//]: # (</details>    )

## Build It Yourself

To build Tᴇɴᴛʀɪs yourself, you need some experience with building C++ projects.

<details><summary> </summary>


### Build Tools

Tᴇɴᴛʀɪs is known to build successfully on Ubuntu 20.04 and newer. 
Building was tested with GCC 10 and clang 10. 

The following packages are required to build Tᴇɴᴛʀɪs:
```
sudo apt install build-essential uuid-dev g++-10 git openjdk-8-jdk python3-pip python3-setuptools python3-wheel
```
Additionally, a recent version of conan is required: 
```
pip3 install --user conan
```

### Dependencies
Most required dependencies are installed via conan. Therefore, Add the respective remotes:
```shell script
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
### Pull & Build
After you installed all dependencies, you are ready to build Tᴇɴᴛʀɪs. 
Make sure you are connected to the internet as Tᴇɴᴛʀɪs downloads things at several points throughout the build processes.

If you did not so far, clone Tᴇɴᴛʀɪs:
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
When you build Tᴇɴᴛʀɪs for the first time, it will take some time. 

The binaries will be located at `tentris/build/bin`. 

### Debug & Test
To compile Tᴇɴᴛʀɪs with debugging symbols, proceed as above but change the cmake command to `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug .. `.

To compile the tests, run `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DTENTRIS_BUILD_TESTS=True .. ` for debugging or `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DTENTRIS_BUILD_TESTS=True .. ` for release.

</details>  
