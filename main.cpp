#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "components/error.h"
#include "components/scanner.h"

namespace {
  static BasicErrorReporter ERROR_REPORTER;

  void run(const std::string& source) {
    Scanner scanner(source, ERROR_REPORTER);
    auto tokens = scanner.scanTokens();

    // For now, just print the tokens.
    for (const auto& token : tokens) {
      std::cout << token.str() <<std::endl;
    }
  }
  
  void runFile(const std::string& path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    run(buffer.str());
    if (ERROR_REPORTER.hadError()) {
      exit(65);
    }
  }

  void runPrompt() {
    while(true) {
      std::cout << "> ";
      std::string line;
      std::getline(std::cin, line);
      run(line);
      ERROR_REPORTER.reset();
    }
  }
}

int main(int argc, char *argv[]) {
  switch(argc) {
    case 1:
      runPrompt(); break;
    case 2:
      runFile(argv[0]); break;
    default:
      std::cout << "Usage: lox [script]" << std::endl;
      return 64;
  }
  return 0;
}
