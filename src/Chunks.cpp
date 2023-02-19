#include "Chunks.hpp"

Chunks::Chunks(request_handler& request) : _request(request) {}

Chunks::~Chunks() {}

/*
 * The handle function in the Chunks class is used to handle a chunked transfer encoding request.
 * It first checks if the request body contains the end marker "0\r\n\r\n", and sets the body_received flag to true if it does.
 * Then it calls the cleanup_query function to remove any unnecessary data from the request.
 * If the request body has been fully received, the function sets the response query parameter to "file uploaded!\n"
 * and closes the chunkfile member variable.
 * Overall, the function handles the reception and processing of chunked transfer encoding requests.
 * */
void Chunks::handle() {
    if (_request.buf.find("0\r\n\r\n") != std::string::npos)
        _request.body_received = true;
    cleanup_query();
    if (_request.body_received) {
        _request.query = "file uploaded!\n";
        _request.chunkfile.close();
    }
}

/*
 * The cleanup_query function in the Chunks class is responsible for cleaning up the input query by parsing the chunks,
 * extracting their length and data, and updating the input buffer accordingly.
 * The function uses local variables to keep track of the start and length of each chunk and uses appropriate string
 * manipulation functions to extract the chunk data.
 * */
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