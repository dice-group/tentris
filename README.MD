# Tᴇɴᴛʀɪs: A Tensor-based Triple Store

<p><img src = "https://tentris.dice-research.org/iswc2020/assets/img/Tentris_logo.svg" alt = "Tᴇɴᴛʀɪs Logo" width = "30%" align = "center"></p>

Tᴇɴᴛʀɪs is a tensor-based RDF triple store with SPARQL support. It is introduced and described in:
> [Alexander Bigerl, Felix Conrads, Charlotte Behning, Mohamed Ahmed Sherif, Muhammad Saleem and Axel-Cyrille Ngonga Ngomo (2020)
**Tentris – A Tensor-Based Triple Store.
** In: The Semantic Web – ISWC 2020](https://tentris.dice-research.org/iswc2020/)

and

> [Alexander Bigerl, Lixi Conrads, Charlotte Behning, Muhammad Saleem and Axel-Cyrille Ngonga Ngomo (2022) Hashing the Hypertrie: Space- and Time-Efficient Indexing for SPARQL in Tensors. In: The Semantic Web – ISWC 2022 Hashing the Hypertrie: Space- and Time-Efficient Indexing for SPARQL in Tensors](https://tentris.dice-research.org/iswc2022/)

## Get It

* download [static prebuilt binaries](https://github.com/dice-group/tentris-private/releases)
  and [try them out](#running-tentris)
* [build it with docker](#docker)

## Running Tᴇɴᴛʀɪs

<details><summary> </summary>

#### Bulk-load Data

Provide an NTRIPLE or TURTLE file to build the an index. By default, the index is stored in the current directory. The
path can be changed with the option `--storage`.

```shell
tentris_loader --file my_nt_file.nt
```

#### Start HTTP endpoint

To start Tᴇɴᴛʀɪs as a HTTP endpoint on port 9080 run now:

```
tentris_server -p 9080
``` 

#### Query

The SPARQL endpoint may now be queried locally at: `127.0.0.1:9080/sparql?query=*your query*`. You can execute queries
with the following curl command:

```shell
curl -G \
--data-urlencode 'query=SELECT * WHERE { ?s ?p ?o . }' \
'127.0.0.1:9080/sparql'
```

If you want to type the query in your browser, the query string must be URL encoded. You can use any online URL encoder
like <https://meyerweb.com/eric/tools/dencoder>.

The following endpoints are available:
Available endpoints:

- HTTP GET `/sparql?query=` for normal queries
- HTTP GET `/stream?query=` for queries with huge results
- HTTP GET `/count?query=` as a workaround for count (consumes a select query)

</details>

## Docker

<details><summary> </summary>

Use the [Dockerfile](./Dockerfile) to build tentris.

* A docker image is available on [docker hub](https://hub.docker.com/r/dicegroup/tentris_server). Get it with
    ```shell script
    docker build -f Dockerfile .
    docker pull dicegroup/tentris_server
    ```

</details>

## Build It Yourself

<details><summary> </summary>

Tᴇɴᴛʀɪs is known to build on Ubuntu 22.04 and newer.
Building was tested with Clang 15. As standard library, only libstdc++11 (v12) was tested. For details
refer to the [Dockerfile](./Dockerfile) or github actions.

</details>
