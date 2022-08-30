#include "error.h"
#include "interpreter.h"
#include <iostream>

void BasicErrorReporter::report(int line, const std::string& where, const std::string& msg) const {
  std::cout << "[line " << line << "] Error" << where << ": " << msg;
  hadError_ = true;
}

void BasicErrorReporter::reportRuntimeError(const RuntimeError& error) {
System.err.println(error.getMessage() +
        "\n[line " + error.token.line + "]");
  hadRuntimeError_ = true;
}
