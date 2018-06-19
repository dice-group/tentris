
#include "einsum/Einsum.hpp"
#include "web/healthCheck.hpp"
#include "pistache/endpoint.h"
#include "pistache/net.h"
#include "pistache/http.h"
#include "pistache/router.h"

using sparsetensor::tensor::shape_t;
using sparsetensor::einsum::raw_subscript;
using sparsetensor::einsum::einsum;


using std::vector;

int main() {
    Pistache::Http::listenAndServe<healthCheck>("*:9999");
}