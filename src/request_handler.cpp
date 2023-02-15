#include "request_handler.hpp"

request_handler::request_handler() : socket(), buf(), method(), file_path(), body_received(false),
    head_received(false), status(0), body_length(0), head_length(0), env(), interpreter(), query(), cookies(0),
    response_sent(false), bytes_sent(0)
{}

request_handler::request_handler(const request_handler &src) :
    socket(src.socket), buf(src.buf), method(src.method), file_path(src.file_path), body_received(src.body_received),
    head_received(src.head_received), status(src.status), body_length(src.body_length), head_length(src.head_length),
    env(src.env), interpreter(src.interpreter), query(src.query), cookies(src.cookies), response_sent(src.response_sent),
    response(src.response), bytes_sent(src.bytes_sent)
{}

request_handler& request_handler::operator= (const request_handler &rhs) {
    if (this != &rhs) {
        socket = rhs.socket;
        buf = rhs.buf;
        method = rhs.method;
        file_path = rhs.file_path;
        body_received = rhs.body_received;
        head_received = rhs.head_received;
        status = rhs.status;
        body_length = rhs.body_length;
        head_length = rhs.head_length;
        env = rhs.env;
        interpreter = rhs.interpreter;
        query = rhs.query;
        cookies = rhs.cookies;
        response = rhs.response;
        bytes_sent = rhs.bytes_sent;
        response_sent = rhs.response_sent;
    }
    return *this;
}

void request_handler::clear() {
    buf.clear();
    method.clear();
    file_path.clear();
    body_received = false;
    head_received = false;
    status = 0;
    body_length = 0;
    head_length = 0;
    env.clear();
    interpreter.clear();
    query.clear();
    cookies = 0;
    response.clear();
    bytes_sent = 0;
    response_sent = false;
}