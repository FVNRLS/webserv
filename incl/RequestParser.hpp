#include "requestHandler.hpp"

#include <string>
#include <vector>

class RequestParser {
 private:
  request_handler& _request;
  location _location_config;
  std::vector<std::string> _allowed_methods;

  void parse_request_line();
  std::vector<std::string> tokenize_request_line();
  void split_url_and_query(std::string& url_and_query_token);
  void parse_body_length();
  void parse_cookies();
  void get_location_id();
  void get_location();
  location& get_location_config();
  location empty_location();
  void get_allowed_methods();
  void check_method();
  std::string get_header_value(std::string const& key);


 public:
  RequestParser(request_handler& request);
  RequestParser(RequestParser const& src);
  RequestParser& operator=(RequestParser const& rhs);
  ~RequestParser();

  void parse();
};