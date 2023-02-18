#include "Chunks.hpp"

Chunks::Chunks(request_handler& request) : _request(request) {}

Chunks::~Chunks() {}

void Chunks::handle() {
    if (_request.buf.find("0\r\n\r\n") != std::string::npos)
        _request.body_received = true;
    cleanup_query();
    if (_request.body_received) {
        _request.query = "file uploaded!\n";
        _request.chunkfile.close();
    }
}

void Chunks::cleanup_query() {
    size_t  start;
    size_t  length;


    while (!_request.buf.empty()) {
        length = std::strtoll(_request.buf.c_str(), NULL, 16);
        if (!length)
            break;
        start = _request.buf.find(CRLF);
        if (start == std::string::npos)
            break;
        start += CRLF.size();
        if (length > _request.buf.size() - start)
            break;
        _request.chunkfile << _request.buf.substr(start, length);
        _request.buf = _request.buf.substr(start + length + CRLF.size());
    }
}