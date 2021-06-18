#include "tentris/store/graphql/GraphQLParser.hpp"
#include "tentris/store/graphql/GraphQLSchema.hpp"

using namespace tentris::store::graphql;

TEST(TestGraphQLSchema, ObjectTypeDefinitions) {
	const std::string schema_str = R"(type Query {
								       person: Person
								   }
								   type Person {
								       name: String
								   })";
	GraphQLSchema schema{};
	tentris::store::graphql::GraphQLParser::parseSchema(schema_str, schema);
	assert(schema.getInterfaceTypeDefinitions().empty());
	assert(schema.getObjectTypeDefinitions().size() == 2);
	assert(schema.getObjectTypeDefinitions().contains("Query"));
	assert(schema.getObjectTypeDefinitions().contains("Person"));
	assert(schema.getObjectTypeDefinitions().at("Query").fields_data.size() == 1);
	assert(schema.getObjectTypeDefinitions().at("Query").fields_data.contains("person"));
	assert(schema.getObjectTypeDefinitions().at("Person").fields_data.size() == 1);
	assert(schema.getObjectTypeDefinitions().at("Person").fields_data.contains("name"));
	assert(schema.getFieldType("name", "Person") == "String");
	assert(schema.getFieldType("person", "Query") == "Person");
	assert(schema.getFieldType("person") == "Person");
}

TEST(TestGraphQLSchema, Directives) {
	const std::string schema_str = R"(type Query {
                                       person: Person
								       company: Company
                                   }
                                   type Person {
                                       name: ID
								       worksAt: Company @inverse @uri(value: "dummy")
                                   }
								   type Company @uri(value: "dummy2") {
								       city: String @uri(value: "dummy3")
								   })";
	GraphQLSchema schema{};
	tentris::store::graphql::GraphQLParser::parseSchema(schema_str, schema);
	assert(schema.getInterfaceTypeDefinitions().empty());
	assert(schema.getObjectTypeDefinitions().size() == 3);
	assert(schema.getObjectTypeDefinitions().at("Query").fields_data.size() == 2);
	assert(schema.getObjectTypeDefinitions().at("Person").fields_data.size() == 2);
	assert(schema.getObjectTypeDefinitions().at("Company").fields_data.size() == 1);
	assert(schema.getFieldType("name", "Person") == "ID");
	assert(schema.getFieldType("worksAt", "Person") == "Company");
	assert(schema.getFieldType("person", "Query") == "Person");
	assert(schema.getFieldType("company", "Query") == "Company");
	assert(schema.getFieldType("city", "Company") == "String");
	assert(schema.getObjectUri("Query").empty());
	assert(schema.getObjectUri("Person").empty());
	assert(schema.getObjectUri("Company") == "dummy2");
	assert(schema.getFieldUri("person", "Query").empty());
	assert(schema.getFieldUri("company", "Query").empty());
	assert(schema.getFieldUri("name", "Person").empty());
	assert(schema.getFieldUri("worksAt", "Person") == "dummy");
	assert(schema.getFieldUri("city", "Company") == "dummy3");
	assert(schema.fieldIsInverse("worksAt", "Person"));
	assert(not schema.fieldIsInverse("name", "Person"));
	assert(not schema.fieldIsInverse("city", "Company"));
}

TEST(TestGraphQLSchema, InterfaceTypeDefinitions) {
	const std::string schema_str = R"(type Query {
                                       animal: Animal
								       dog: Dog
                                       cat: Cat
                                   }
                                   interface Animal @uri(value: "dummy") {
                                       name: String
                                   }
								   type Dog implements Animal  {
								      name: String
                                      furColor: String
								   }
                                   type Cat implements Animal @uri(value: "dummy2") {
								       name: String
								       eyeColor: String
								   })";
	GraphQLSchema schema{};
	tentris::store::graphql::GraphQLParser::parseSchema(schema_str, schema);
    assert(schema.getInterfaceTypeDefinitions().size() == 1);
    assert(schema.getObjectTypeDefinitions().size() == 3);
    assert(schema.getObjectTypeDefinitions().at("Query").fields_data.size() == 3);
    assert(schema.getObjectTypeDefinitions().at("Dog").fields_data.size() == 2);
    assert(schema.getObjectTypeDefinitions().at("Cat").fields_data.size() == 2);
    assert(schema.getInterfaceTypeDefinitions().at("Animal").fields_data.size() == 1);
	assert(schema.getObjectUri("Animal") == "dummy");
	assert(schema.getObjectUri("Cat") == "dummy2");
	assert(schema.getFieldType("name", "Animal") == "String");
	assert(schema.getFieldType("name", "Dog") == "String");
	assert(schema.getFieldType("name", "Cat") == "String");
	assert(schema.getObjectTypeDefinitions().at("Dog").implementations == std::set<std::string>{"Animal"});
	assert(schema.getObjectTypeDefinitions().at("Cat").implementations == std::set<std::string>{"Animal"});
}

TEST(TestGraphQLSchema, FieldArguments) {
	const std::string schema_str = R"(type Query {
                                       animal: Animal
								       dog(furColor: String): Dog
                                       cat(eyeColor: String): Cat
                                   }
                                   interface Animal @uri(value: "dummy") {
                                       name: String
                                   }
								   type Dog implements Animal  {
								      name: String
                                      furColor: String
								   }
                                   type Cat implements Animal @uri(value: "dummy2") {
								       name: String
								       eyeColor: String
								   })";
	GraphQLSchema schema{};
	tentris::store::graphql::GraphQLParser::parseSchema(schema_str, schema);
	assert(schema.getArgumentType("furColor", "dog", "Query") == "String");
	assert(schema.getArgumentType("eyeColor", "cat", "Query") == "String");
    ASSERT_THROW(schema.getArgumentType("eyeColor", "dog", "Query"), SchemaException);
    ASSERT_THROW(schema.getArgumentType("name", "animal", "Query"), SchemaException);
}

TEST(TestGraphQLSchema, WrappingTypesAndSchemaException) {
    const std::string schema_str = R"(type Query {
                                       people: [Person]!
								       companies: [Company]
                                   }
                                   type Person {
                                       name: ID!
								       worksAt: Company @inverse @uri(value: "dummy")
                                   }
								   type Company @uri(value: "dummy2") {
								       city: String @uri(value: "dummy3")
								   })";
    GraphQLSchema schema{};
    tentris::store::graphql::GraphQLParser::parseSchema(schema_str, schema);
	assert(schema.fieldIsList("people"));
	assert(schema.fieldIsList("companies"));
	assert(schema.fieldIsNonNull("people"));
	assert(not schema.fieldIsNonNull("companies"));
	assert(schema.fieldIsNonNull("name", "Person"));
	assert(schema.fieldIsScalar("name", "Person"));
	assert(not schema.fieldIsScalar("worksAt", "Person"));
    ASSERT_THROW(schema.fieldIsInverse("nam", "Person"), SchemaException);
    ASSERT_THROW(schema.getFieldUri("nam", "Person"), SchemaException);
    ASSERT_THROW(schema.getObjectUri("Compny"), SchemaException);
    ASSERT_THROW(schema.getFieldUri("cty", "Compny"), SchemaException);
    ASSERT_THROW(schema.fieldIsNonNull("wors", "Person"), SchemaException);
}