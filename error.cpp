#include "error.h"
#include <iostream>

void BasicErrorReporter::report(int line, const std::string& where, const std::string& msg) {
  std::cout << "[line " << line << " Error" << where << ": " << msg;
  hadError_ = true;
}
