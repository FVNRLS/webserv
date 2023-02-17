#include "handleChunks.hpp"

handleChunks::handleChunks(request_handler& request) : _request(request) {}

handleChunks::~handleChunks() {}

void handleChunks::handle() {
    if (_request.chunk_complete) {
        _request.chunk_length = std::strtol(_request.buf.c_str(), NULL, 16);
        if (_request.chunk_length == 0) {
            _request.body_received = true;
            return;
        }
        trim_first_line(_request.buf);
    }
    if (_request.buf.find(END_OF_REQUEST) == std::string::npos) {
        _request.chunk_complete = false;
        _request.chunked_body += _request.buf;
        _request.buf.clear();
        return;
    }
    _request.chunked_body += _request.buf.substr(0, _request.chunk_length);
    if (_request.buf.size() != _request.chunked_body.size() + 2) {
        _request.buf = _request.buf.substr(_request.chunk_length + 2);
        handle();
    }
    _request.buf.clear();
}
