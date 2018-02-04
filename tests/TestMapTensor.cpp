#include <gtest/gtest.h>

#include "tensor/MapTensor.hpp"
#include <tuple>

TEST(TestMapTensor, single_write_read) {
    using namespace sparsetensor::tensor;
    Key_t key{5, 10, 8};
    short value = 1;

    MapTensor<short> tensor{shape_t{64, 64, 64}};

    tensor.set(key, value);

    const short saved_value = tensor.get(key);

    ASSERT_EQ(saved_value, value);
}

TEST(TestMapTensor, double_write_read0) {
    using namespace sparsetensor::tensor;
    // data
    Key_t key{5, 10, 8};
    short value1 = 3;
    short value2 = 1;

    // init
    MapTensor<short> tensor{shape_t{64, 64, 64}};

    // load data
    tensor.set(key, value1);
    tensor.set(key, value2);

    // validate
    short saved_value = tensor.get(key);

    ASSERT_EQ(saved_value, value2);
}

TEST(TestMapTensor, mult_write_read1) {
    using namespace sparsetensor::tensor;
    // data
    vector<Key_t> keys{
            {0, 10, 8},
            {0, 10, 9}
    };
    vector<short> values{
            1,
            1
    };

    // init
    MapTensor<short> tensor{shape_t{64, 64, 64}};

    // load data
    for (size_t i = 0; i < keys.size(); ++i) {
        const Key_t &key = keys[i];
        short &value = values[i];

        tensor.set(key, value);
    }

    // validate
    for (size_t i = 0; i < keys.size(); ++i) {
        const Key_t &key = keys[i];
        short &value = values[i];

        short saved_value = tensor.get(key);

        ASSERT_EQ(saved_value, value);
    }
}

TEST(TestMapTensor, test_mult_write_read4) {
    using namespace sparsetensor::tensor;
    // data
    vector<Key_t> keys{
            {0, 10, 8,  2},
            {2, 5,  10, 9},
            {1, 0,  10, 5},
            {3, 4,  3,  0},
            {4, 2,  2,  6},
            {2, 3,  3,  2},
            {1, 5,  3,  0},
            {6, 5,  6,  8},
            {1, 9,  7,  3}
    };
    short value = 1;

    // init
    MapTensor<short> tensor{shape_t{64, 64, 64, 64}};

    // load data
    for (const Key_t &key :keys) {
        tensor.set(key, value);
    }

    // validate
    for (vector<uint64_t> &key :keys) {
        const short saved_value = tensor.get(key);

        ASSERT_EQ(saved_value, value);
    }
}

TEST(TestMapTensor, sum) {
    using namespace sparsetensor::tensor;
    // data
    vector<Key_t> keys{
            {0, 10, 8, 2},
            {0, 11, 8, 2},
            {1, 10, 8, 2},
            {2, 10, 8, 1},
            {2, 10, 8, 2}
    };

    vector<short> values{
            1,
            1,
            3,
            1,
            1
    };

    // init
    MapTensor<short> tensor{shape_t{64, 64, 64, 64}};

    // load data
    for (int i = 0; i < keys.size(); ++i) {
        const Key_t &key = keys[i];
        const short &value = values[i];
        tensor.set(key, value);
    }

    ASSERT_EQ(std::accumulate(values.begin(), values.end(), 0), tensor.sum);

}

TEST(TestMapTensor, write_read_delete_read) {
    using namespace sparsetensor::tensor;
    // init tensor
    MapTensor<int> tensor{shape_t{3, 3, 3}};
    std::cout << tensor << std::endl;

    // write (1,1,1) -> 4
    Key_t coord{1, 1, 1};
    int value{4};
    tensor.set(coord, value);
    std::cout << tensor << std::endl;

    // read (1,1,1)
    int retrieved_value = tensor.get(coord);
    ASSERT_EQ(retrieved_value, value);

    // write (0,0,0) -> 4
    int zero{};
    tensor.set(coord, zero);
    std::cout << tensor << std::endl;

    // read (1,1,1)
    retrieved_value = tensor.get(coord);
    ASSERT_EQ(retrieved_value, zero);
}

TEST(TestMapTensor, zero_dim_write_read_delete_read) {
    using namespace sparsetensor::tensor;
    // init tensor
    MapTensor<int> tensor{shape_t{}};
    std::cout << tensor << std::endl;

    // write (1,1,1) -> 4
    Key_t coord{};
    int value{4};
    tensor.set(coord, value);
    std::cout << tensor << std::endl;

    // read (1,1,1)
    int retrieved_value = tensor.get(coord);
    ASSERT_EQ(retrieved_value, value);

    // write (0,0,0) -> 4
    int zero{};
    tensor.set(coord, zero);
    std::cout << tensor << std::endl;

    // read (1,1,1)
    retrieved_value = tensor.get(coord);
    ASSERT_EQ(retrieved_value, zero);
}

TEST(TestMapTensor, test_iterator) {
    using namespace sparsetensor::tensor;
    // data
    vector<Key_t> keys{
            {0, 10, 8,  2},
            {2, 5,  10, 9},
            {1, 0,  10, 5},
            {3, 4,  3,  0},
            {4, 2,  2,  6},
            {2, 3,  3,  2},
            {1, 5,  3,  0},
            {6, 5,  6,  8},
            {1, 9,  7,  3}
    };
    short value = 1;

    // init
    MapTensor<short> tensor{shape_t{64, 64, 64, 64}};

    // load data
    for (const Key_t &key :keys) {
        tensor.set(key, value);
    }

    // validate
    for (const auto &[key, saved_value] :tensor) {
        std::cout << key << std::endl;
        // ASSERT_TRUE(std::find(keys.begin(), keys.end(), key) != keys.end());
        ASSERT_EQ(value, saved_value);
    }
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}