#ifndef SPARSETENSOR_TENSOR_CROSSPRODUCTTENSOR_HPP
#define SPARSETENSOR_TENSOR_CROSSPRODUCTTENSOR_HPP

#include "../einsum/Subscript.hpp"
#include "../einsum/Types.hpp"
#include "../hypertrie/Types.hpp"
#include "../einsum/ShapeCalc.hpp"
#include "Tensor.hpp"
#include "MapTensor.hpp"
#include <tuple>

using sparsetensor::einsum::calcShape;
using sparsetensor::einsum::Subscript;
using sparsetensor::einsum::label_pos_t;
using sparsetensor::einsum::label_t;
using sparsetensor::einsum::op_pos_t;
using std::tuple;
using std::vector;


namespace sparsetensor::tensor {

    template<typename T>
    class CrossProductTensor;

    template<typename T>
    class Iterator<T, CrossProductTensor>;

    /**
     * A Tensor that represents a cross product defined by a fitting subscript of several input tensors.
     * @tparam T
     */
    template<typename T>
    class CrossProductTensor : public Tensor<T, CrossProductTensor> {
        friend class Iterator<T, CrossProductTensor>;

    public:
        /**
         * Pointers to the Tensors that shall be inputed into this cross product.
         */
        vector<MapTensor<T> *> input_tensors{};
        /**
         * a Mapping tensor_input_pos -> (input_label_pos -> result_label_pos).
         */
        vector<vector<label_pos_t>> posss_in_result{};

        /**
         * Construct empty CrossProductTensor.
         * @param shape shape of the tensor to be created.
         */
        explicit CrossProductTensor(const shape_t shape)
                : Tensor<T, CrossProductTensor>{shape} {}

        /**
         * Create a CrossProductTensor from tensors and an subscript.
         * @param input_tensors tensors to cross
         * @param subscript mapping of the crossing
         */
        CrossProductTensor(const vector<MapTensor<T> *> &input_tensors, const Subscript &subscript)
                : Tensor<T, CrossProductTensor>{} {
            this->setShape(calcShape<T, MapTensor>(input_tensors, subscript));


            const map<op_pos_t, vector<label_t>> &operands_labels = subscript.getOperandsLabels();
            const unordered_map<label_t, label_pos_t> &label_pos_in_result = subscript.getLabelPosInResult();

            // check if there is any input
            if (input_tensors.size() == 0) {
                return;
            }

            // check if any input is zero
            for (const MapTensor<T> *input : input_tensors) {
                if (input->nnz == 0) {
                    return;
                }
            }

            // sort input_tensors into tensors and scalars
            for (size_t i = 0; i < input_tensors.size(); ++i) {
                const vector<label_t> &labels = operands_labels.at(op_pos_t(i));
                MapTensor<T> *input = input_tensors[i];
                this->input_tensors.push_back(input);

                // translate key_pos in input to output key_pos
                vector<label_pos_t> op_labels_pos_in_result{};
                for (const label_t &label: labels) {
                    op_labels_pos_in_result.push_back(label_pos_in_result.at(label));
                }
                posss_in_result.push_back(op_labels_pos_in_result);

            }

            // initialize Tensor properties
            this->nnz = 1;
            for (const MapTensor<T> *tensor_input: this->input_tensors) {
                this->nnz *= tensor_input->nnz;
                this->sum *= tensor_input->sum; // todo: is that true?
            }
        }


        virtual T get(const Key_t key) const override {
            throw "Not yet implemented.";
        }

        virtual void set(const Key_t key, const T value) override {
            throw "Set not supported by CrossProductTensor.";
        }

        /**
         * Get an empty CrossProductTensor of given shape.
         * @param shape shape of the tensor
         * @return new empty CrossProductTensor
         */
        static CrossProductTensor *getZero(const shape_t &shape) {
            return new CrossProductTensor(shape);
        }

        Iterator<T, CrossProductTensor> begin() {
            return Iterator<T, CrossProductTensor>(*this);
        }

        Iterator<T, CrossProductTensor> end() {
            return Iterator<T, CrossProductTensor>(*this, this->nnz);
        }
    };

    template<typename T>
    class Iterator<T, CrossProductTensor> {
    public:
        Iterator(CrossProductTensor<T> &container, uint64_t id = 0) :
                container(container),
                id(id),
                end_id(container.nnz),
                key(Key_t(container.ndim)) {
            for (const auto &input : container.input_tensors) {
                input_iters.push_back(input->begin());
            }
        }

        /**
         * Copy-Constructor
         * @param other Iterator to be copied
         */
        Iterator(const Iterator<T, CrossProductTensor> &other) :
                container(other.container),
                id(other.id),
                end_id(other.end_id),
                key(other.key) {}

    private:
        /**
         * Reference to the iterated CrossProductTensor.
         */
        CrossProductTensor<T> &container;
        /**
         * id of the current entry from container.
         */
        uint64_t id;
        /**
         * end id of container.end().
         */
        uint64_t end_id;
        /**
         * Iterators for the containers inputs.
         */
        vector<Iterator<T, MapTensor>> input_iters{};

        Key_t key;
        T value{};

    public:
        Iterator<T, CrossProductTensor> &operator++() {
            id++;
            if (id != end_id) {

                for (int i = 0; i < input_iters.size(); ++i) {
                    Iterator<T, MapTensor> &input_iter = input_iters[i];

                    ++input_iter;
                    if (input_iter == container.input_tensors[i]->end()) {
                        if (i == input_iters.size() - 1) {
                            // last iterator shall be flipped back it means the end is reached.
                            // should never be reached.
                            throw ("something is fishy");
                            break;
                        }
                        // todo: sort the zero-dim tensors to the end to avoid unnecessary tensor.begin() calls
                        input_iters[i] = container.input_tensors[i]->begin();
                    } else {
                        break;
                    }
                }
            }
            return *this;
        }

        Iterator<T, CrossProductTensor> operator++(int) {
            operator++();
            return *this;
        }

        tuple<Key_t, T> operator*() {
            // reset the value
            value = 1;

            // input-to-output key
            auto poss_in_result_ = container.posss_in_result.cbegin();
            // iterate inputs
            for (Iterator<T, MapTensor> input_iter : input_iters) {
                const auto &
                [input_key, input_value] = *input_iter;
                // set the value
                value *= input_value;
                // set key at right positions
                const vector<label_pos_t> &poss_in_result = *poss_in_result_;
                for (int j = 0; j < input_key.size(); ++j) {
                    const label_pos_t &pos_in_result = poss_in_result[j];
                    const key_part_t &key_part = input_key[j];
                    key[pos_in_result] = key_part;
                }
                ++poss_in_result_;
            }

            return {key, value};
        }

        bool operator==(const Iterator<T, CrossProductTensor> &rhs) const {
            return rhs.id == id;
        }

        bool operator!=(const Iterator<T, CrossProductTensor> &rhs) const {
            return rhs.id != id;
        }
    };


}
#endif //SPARSETENSOR_TENSOR_CROSSPRODUCTTENSOR_HPP
