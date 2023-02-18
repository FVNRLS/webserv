#pragma once

#include "request_handler.hpp"

class Chunks {
private:
    request_handler             &_request;

    void                        cleanup_query();

public:
    Chunks(request_handler &request);
    ~Chunks();

    void                handle();
};

