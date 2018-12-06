#include <gtest/gtest.h>

#include <random>

#include <tnt/util/container/VecMap.hpp>


namespace {
    using namespace tnt::util::container;
}


TEST(TestVecMap, loadAndCkeckdataAlsoSizeOfVecmap) {
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',};

    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }

    // check if data was loaded correctly
    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_EQ(vecMap.get(keys[i]), values[i]);
    }

    ASSERT_EQ(keys.size(), vecMap.size());
}

TEST(TestVecMap, loadDataAndDeleteAllOfThem) {
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};

    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }

    std::vector<int> shuffle{keys};
    std::shuffle(shuffle.begin(), shuffle.end(), std::mt19937(std::random_device()()));
    for (size_t j = 0; j < shuffle.size(); ++j) {
        int key = shuffle[j];
        vecMap.delItem(key);
        ASSERT_EQ(vecMap.get(key), std::numeric_limits<char>().max());
        ASSERT_EQ(vecMap.size(), keys.size() - j - 1);
    }
}


TEST(TestVecMap, deleteNotExistedItems) {
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};

    // Load data
    VecMap<int, char> vecMap{};

    vecMap.delItem(1); //not existed, ==> should nothing happened

    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }

    vecMap.delItem(1);//existed
    vecMap.delItem(1);//not existed, ==> should nothing happened
    vecMap.delItem(15);//not existed, ==> should nothing happened
}

TEST(TestVecMap, replaceSomeElements) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    std::vector<char> revValues{values};
    std::reverse(revValues.begin(), revValues.end());
    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }

    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], revValues[i]);
    }

    ASSERT_EQ(keys.size(), vecMap.size());

    // check if data was replaced correctly
    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_EQ(vecMap.get(keys[i]), revValues[i]);
    }
}

TEST(TestVecMap, addAndRemoveInterchangably) {
    enum Operator {
        ADD, REMOVE
    };

    int len = 1000;

    VecMap<int, int> vecMap;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis_operator(0, 100);
    std::uniform_int_distribution dis_number(0, 10000);
    std::uniform_real_distribution dis_index(0.0, 1.0);
    std::map<int, int> mp;

    for (int i = 0; i < len; i++) {
        Operator op = Operator::ADD;
        int opProb = dis_operator(gen);
        if (opProb < 10) //10 percentage for deduction
            op = Operator::REMOVE;
        switch (op) {
            case Operator::ADD: {
                int key = dis_number(gen); //maybe key is repetitive and the value must be replaced
                int value = dis_number(gen);
                mp[key] = value;
                vecMap.setItem(key, value);
                break;
            }
            case Operator::REMOVE: {
                if (mp.empty()) continue;
                double r = dis_index(gen);
                auto index = static_cast<int>(mp.size() * r);
                auto iterator = mp.begin();
                std::advance(iterator, index);
                auto key = (iterator->first);
                mp.erase(iterator);
                vecMap.delItem(key);
                break;
            }
        }
        ASSERT_EQ(mp.size(), vecMap.size());
        for (auto iter = mp.begin(); iter != mp.end(); iter++)
            ASSERT_EQ(iter->second, vecMap.get(iter->first));
    }
}


TEST(TestVecMap, minMax) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};

    for (int turn = 0; turn < 10; turn++) {
        std::vector<int> shuffle_key{keys};
        std::shuffle(shuffle_key.begin(), shuffle_key.end(), std::mt19937(std::random_device()()));

        std::set<int> s;

        VecMap<int, char> vecMap;
        ASSERT_EQ(vecMap.min(), std::numeric_limits<int>::max());
        ASSERT_EQ(vecMap.max(), std::numeric_limits<int>::min());
        for (unsigned int i = 0; i < shuffle_key.size(); i++) {
            s.insert(shuffle_key[i]);
            vecMap.setItem(shuffle_key[i], values[i]);
            ASSERT_EQ(vecMap.min(), *(s.cbegin()));
            ASSERT_EQ(vecMap.max(), *(s.crbegin()));
        }
    }
}

TEST(TestVecMap, out_of_range) {
//    try {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }
    ASSERT_THROW(vecMap.at(15), std::out_of_range);
}

TEST(TestVecMap, out_of_range_lowHigh) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }

    ASSERT_THROW(vecMap.at(6, 0, 6), std::out_of_range);
}

TEST(TestVecMap, get) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }
    for (unsigned int i = 0; i < keys.size(); i++)
        ASSERT_EQ(vecMap.get(keys[i]), values[i]);
}

TEST(TestVecMap, getLowHigh) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }
    for (unsigned int i = 0; i < keys.size(); i++)
        ASSERT_EQ(vecMap.get(keys[i], 0, keys.size()), values[i]);


    for (unsigned int i = 1; i < keys.size(); i++)
        ASSERT_EQ(vecMap.get(keys[i], 0, i - 1), std::numeric_limits<char>::max());


    for (unsigned int i = 0; i < keys.size(); i++)
        ASSERT_EQ(vecMap.get(keys[i], 0, i), std::numeric_limits<char>::max());

    for (unsigned int i = 0; i < keys.size(); i++)
        ASSERT_EQ(vecMap.get(keys[i], 0, i + 1), values[i]);
}

TEST(TestVecMap, contains) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }

    for (int key : keys) {
        ASSERT_TRUE(vecMap.contains(key));
    }

    for (int i = 15; i < 30; i++)
        ASSERT_FALSE(vecMap.contains(i));

    std::vector<int> shuffle_key(keys);
    std::shuffle(shuffle_key.begin(), shuffle_key.end(), std::mt19937(std::random_device()()));

    std::set<int> keySets;
    for (int key : keys) {
        keySets.insert(key);
    }

    for (int i : shuffle_key) {
        vecMap.delItem(i);
        keySets.erase(i);
        for (int keySet : keySets)
            ASSERT_TRUE(vecMap.contains(keySet));
        ASSERT_FALSE(vecMap.contains(i));
    }
}

TEST(TestVecMap, keyByInd) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }

    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_EQ(keys[i], vecMap.keyByInd(i));
    }

    ASSERT_THROW(vecMap.keyByInd(100), std::out_of_range);
}

TEST(TestVecMap, valByInd) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // Load data
    VecMap<int, char> vecMap{};
    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
    }

    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_EQ(values[i], vecMap.valByInd(i));
    }

    ASSERT_THROW(vecMap.valByInd(100), std::out_of_range);
}

TEST(TestVecMap, keysAndValues) {
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // Load data
    std::vector<int> localKeys;
    std::vector<char> localValues;

    VecMap<int, char> vecMap{};

    for (size_t i = 0; i < keys.size(); ++i) {
        vecMap.setItem(keys[i], values[i]);
        localKeys.push_back(keys[i]);
        localValues.push_back(values[i]);
        ASSERT_EQ(localKeys, vecMap.keys());
        ASSERT_EQ(localValues, vecMap.values());
    }
}

