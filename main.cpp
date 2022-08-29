#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <cstdlib>
#include "components/error.h"
#include "components/scanner.h"
#include "components/expr.h"
#include "components/astprinter.h"

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
      Token t(TokenType::MINUS, "-", std::any(), 1);
      auto lit = std::make_unique<Literal>(123);
      auto u = std::make_unique<Unary>(t, std::move(lit));
      Token t2(TokenType::STAR, "*", std::any(), 1);
      auto lit2 = std::make_unique<Literal>(45.66);
      auto g = std::make_unique<Grouping>(std::move(lit2));
      auto b =std::make_unique<Binary>(std::move(u), t2, std::move(g));
      AstPrinter p;
      std::cout << p.print(*b) << std::endl;
      std::cout << "Usage: lox [script]" << std::endl;
      return 64;
  }
  return 0;
}
