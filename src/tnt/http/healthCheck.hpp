#ifndef HEALTHCHECK_HPP
#define HEALTHCHECK_HPP

#include <pistache/endpoint.h>

//using namespace Pistache;

class healthCheck : public Pistache::Http::Handler {
public:
    HTTP_PROTOTYPE(healthCheck)

    void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
        response.send(Pistache::Http::Code::Ok, "Hello World");
    }
};

#endif //HEALTHCHECK_HPP
