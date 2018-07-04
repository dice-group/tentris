#include <gtest/gtest.h>
#include <iostream>
#include "store/RDF/Node.hpp"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using namespace tnt::store;

TEST(Node, polymorphism) {
    const char *string = "\"asdasd\"^^<dsa>";
    std::string lit{string};
    std::shared_ptr<Node> instance{new Literal{lit}};
    Node::NodeType type = instance->type();
    std::cout << type << std::endl;
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
