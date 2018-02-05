#ifndef SPARSETENSOR_TENSOR_CROSSPRODUCTTENSOR_HPP
#define SPARSETENSOR_TENSOR_CROSSPRODUCTTENSOR_HPP

#include "../einsum/Subscript.hpp"
#include "../einsum/Types.hpp"
#include "../hypertrie/Types.hpp"
#include "Tensor.hpp"
#include <tuple>


using sparsetensor::einsum::Subscript;
using sparsetensor::einsum::label_pos_t;
using sparsetensor::einsum::label_t;
using sparsetensor::einsum::op_pos_t;
using std::tuple;
using std::vector;


namespace sparsetensor::tensor {
    /**
     * A Tensor that represents a cross product defined by a fitting subscript of several input tensors.
     * @tparam T
     */
    template<typename T>
    class CrossProductTensor : public Tensor<T> {
    public:
        /**
         * Construct empty CrossProductTensor.
         * @param shape shape of the tensor to be created.
         */
        explicit CrossProductTensor(const shape_t shape)
                : Tensor<T>{shape} {}

        /**
         * Create a CrossProductTensor from tensors and an subscript.
         * @param input_tensors tensors to cross
         * @param subscript mapping of the crossing
         */
        CrossProductTensor(vector<Tensor<T> *> &input_tensors, Subscript &subscript)
                : Tensor<T>{} {
            // todo: calc shape
            shape_t shape{};
            this->setShape(shape);


            const map<op_pos_t, vector<label_t>> &operands_labels = subscript.getOperandsLabels();
            const unordered_map<label_t, label_pos_t> &label_pos_in_result = subscript.getLabelPosInResult();

            // check if there is any input
            if (input_tensors.size() == 0) {
                return;
            }

            // check if any input is zero
            for (const Tensor<T> *&input : input_tensors) {
                if (input->nnz == 0) {
                    return;
                }
            }

            // sort input_tensors into tensors and scalars
            for (size_t i = 0; i < input_tensors.size(); ++i) {
                const vector<label_t> &labels = operands_labels.at(op_pos_t(i));
                Tensor<T> *&input = input_tensors[i];
                this->input_tensors.push_back(input);

                // translate key_pos in input to output key_pos
                vector<label_pos_t> op_labels_pos_in_result{};
                for (label_t &label: labels) {
                    op_labels_pos_in_result.push_back(label_pos_in_result.at(label));
                }
                posss_in_result.push_back(op_labels_pos_in_result);

            }

            // initialize Tensor properties
            this->nnz = 1;
            for (const Tensor<T> *&tensor_input: this->input_tensors) {
                this->nnz *= tensor_input->nnz;
                this->sum *= tensor_input->sum; // todo: is that true?
            }
        }


    private:
        /**
         * Pointers to the Tensors that shall be inputed into this cross product.
         */
        vector<Tensor<T> *> input_tensors{};
        /**
         * a Mapping tensor_input_pos -> (input_label_pos -> result_label_pos).
         */
        vector<vector<label_pos_t>> posss_in_result{};

    public:

        T get(Key_t &key) override {
            throw "Not yet implemented.";
        }

        void set(Key_t &key, T &value) override {
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

        class Iterator : public Tensor<T>::Iterator {
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
            Iterator(const Iterator &other) :
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
            vector<typename Tensor<T>::Iterator> input_iters{};

            vector<key_part_t> key;
            T value{};

        public:
            typename Tensor<T>::Iterator &operator++() override {
                if (id != end_id) {
                    id++;

                    for (int i = 0; i < input_iters.size(); ++i) {
                        typename Tensor<T>::Iterator &input_iter = input_iters[i];

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

            typename Tensor<T>::Iterator operator++(int) override {
                operator++();
                return *this;
            }

            tuple<Key_t, T> operator*() override {
                // reset the value
                value = 1;

                // input-to-output key
                auto poss_in_result_ = container.posss_in_result.cbegin();
                container.input_tensors[0];
                // iterate inputs
                for (typename Tensor<T>::Iterator input_iter : input_iters) {
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

            bool operator==(const typename Tensor<T>::Iterator &rhs) const override {
                return static_cast<const Iterator &>(rhs).id == id;
            }

            bool operator!=(const typename Tensor<T>::Iterator &rhs) const override {
                return static_cast<const Iterator &>(rhs).id != id;
            }
        };

        typename Tensor<T>::Iterator begin() override {
            return Iterator(*this);
        }

        typename Tensor<T>::Iterator end() override {
            return Iterator(*this, this->nnz);
        }
    };

}
#endif //SPARSETENSOR_TENSOR_CROSSPRODUCTTENSOR_HPP
