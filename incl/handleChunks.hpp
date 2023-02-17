#pragma once

#include "request_handler.hpp"

class handleChunks {
private:
    request_handler             &_request;

public:
    handleChunks(request_handler &request);
    ~handleChunks();

    void                handle();
};

