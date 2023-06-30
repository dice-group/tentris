This branch provides the GraphQL service presented in the paper
> Nikolaos Karalis, Alexander Bigerl,  and Axel-Cyrille Ngonga Ngomo: Native Execution of GraphQL Queries over RDF Graphs using Multi-way Joins. In: SEMANTiCS 2023

The implementation of the left-join algorithm presented in the paper is available [here](https://github.com/dice-group/hypertrie/tree/left-join)

### GraphQL directives provided by our service
- @uri(value: String): Maps GraphQL types and fields of a GraphQL schema to RDF IRIs. Example: type Person @uri(value: "<http://xmlns.com/foaf/0.1/Person>") { ... }
- @inverse: Denotes that the inverse direction of a field's property should be used.
- @filter: Denotes that the results of a particular field should be filtered using that field's type.