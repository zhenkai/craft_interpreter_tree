#include "error.h"
#include <iostream>

void BasicErrorReporter::report(int line, const std::string& where, const std::string& msg) const {
  std::cout << "[line " << line << "] Error" << where << ": " << msg;
  hadError_ = true;
}
