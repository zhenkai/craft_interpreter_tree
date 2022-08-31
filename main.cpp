#include "components/astprinter.h"
#include "components/error.h"
#include "components/expr.h"
#include "components/interpreter.h"
#include "components/parser.h"
#include "components/scanner.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

namespace {
static BasicErrorReporter ERROR_REPORTER;
Interpreter ip(ERROR_REPORTER);

void run(const std::string &source) {
  Scanner scanner(source, ERROR_REPORTER);
  auto tokens = scanner.scanTokens();
  Parser parser(tokens, ERROR_REPORTER);

  auto stmts = parser.parse();

  if (ERROR_REPORTER.hadError()) {
    return;
  }

  ip.interpret(stmts);
}

void runFile(const std::string &path) {
  std::ifstream t(path);
  std::stringstream buffer;
  buffer << t.rdbuf();
  run(buffer.str());
  if (ERROR_REPORTER.hadError()) {
    exit(65);
  }
  if (ERROR_REPORTER.hadRuntimeError()) {
    exit(70);
  }
}

void runPrompt() {
  while (true) {
    std::cout << "> ";
    std::string line;
    std::getline(std::cin, line);
    run(line);
    ERROR_REPORTER.reset();
  }
}
} // namespace

int main(int argc, char *argv[]) {
  switch (argc) {
  case 1:
    runPrompt();
    break;
  case 2:
    runFile(argv[0]);
    break;
  default:
    std::cout << "Usage: lox [script]" << std::endl;
    return 64;
  }
  return 0;
}
