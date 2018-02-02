#include "einsum/Subscript.hpp"

#define BOOST_TEST_MODULE LibSparseTensorTest

#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(TestSubscript)

    BOOST_AUTO_TEST_CASE(print_and_check_norming) {
        using namespace ::sparsetensor::einsum;
        // data
        vector<vector<label_t >> raw_op_sc{
                {3, 2, 1},
                {1, 2, 3, 2},
                {0}
        };

        // print
        std::cout << "raw_op_labels:\n\t" << raw_op_sc << std::endl;

        vector<label_t> raw_res_sc{3, 0};
        std::cout << "raw_res_labels:\n\t" << raw_res_sc << std::endl;

        Subscript sc{raw_op_sc, raw_res_sc};
        std::cout << sc << std::endl;

        // check normed labels
        auto all_labels = std::unordered_set<label_t>{0, 1, 2, 3};
        BOOST_CHECK(sc.getAllLabels() == all_labels);

        vector<label_t> op_labels0{0, 1, 2};
        BOOST_CHECK(op_labels0 == sc.getOperandsLabels().at(0));

        vector<label_t> op_labels1{2, 1, 0, 1};
        BOOST_CHECK(op_labels1 == sc.getOperandsLabels().at(1));

        vector<label_t> op_labels2{3};
        BOOST_CHECK(op_labels2 == sc.getOperandsLabels().at(2));

        vector<label_t> res_labels{0, 3};
        BOOST_CHECK(res_labels == sc.getResultLabels());
    }

    BOOST_AUTO_TEST_CASE(optimize) {
        using namespace ::sparsetensor::einsum;
        // data
        vector<vector<label_t >> raw_op_sc{
                {3, 2, 1},
                {1, 2, 3, 2},
                {0}
        };

        vector<label_t> raw_res_sc{3, 0};

        Subscript opt_sc = Subscript{raw_op_sc, raw_res_sc}.optimize();

        std::cout << opt_sc << std::endl;

        for (auto entry : opt_sc.getSubSubscripts()) {
            std::cout << entry.second << std::endl;
        }

    }


BOOST_AUTO_TEST_SUITE_END()