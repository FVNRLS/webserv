#include <cstdlib>
#include <iostream>
#include <string>

int main() {
  std::string s("");
  std::cout << std::strtoll(s.c_str(), NULL, 10);
}