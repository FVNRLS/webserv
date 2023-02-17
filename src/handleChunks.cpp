#include "handleChunks.hpp"

handleChunks::handleChunks(request_handler& request) : _request(request) {}

handleChunks::~handleChunks() {}

void handleChunks::handle() {
    if (!_request.chunkeeeeeeeeeeeeeeee) {
    size_t chunk_length = std::strtol(_request.buf.c_str(), NULL, 16);
    if (chunk_length == 0) {
        _request.body_received = true;
        return ;
    }
    trim_first_line(_request.buf);
    if (_request.buf.find(END_OF_REQUEST) == std::string::npos)
        _request.chunk_is_not_complete = true;
    trim_endofrequest(_request.buf);
    _request.chunked_body += _request.buf;
    _request.buf.clear();
}
