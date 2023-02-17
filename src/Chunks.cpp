#include "Chunks.hpp"

Chunks::Chunks(request_handler& request) : _request(request) {}

Chunks::~Chunks() {}

void Chunks::handle() {
    if (_request.status)
        return;
    _request.response.clear();
    if (_request.buf.find("0\r\n\r\n") != std::string::npos)
        _request.body_received = true;
    cleanup_query();
}

void Chunks::cleanup_query() {
    size_t  start;
    size_t  length;


    while (!_request.buf.empty()) {
        length = std::strtoll(_request.buf.c_str(), NULL, 16);
        if (length == 0LL)
            break;
        start = _request.query.find(CRLF);
        if (start == std::string::npos)
            break;
        start += CRLF.size();
        _request.chunkfile << _request.buf.substr(start, length);
        _request.buf = _request.buf.substr(start + length + CRLF.size());
    }
    if (_request.body_received)
        _request.query = "file uploaded!\n";
}