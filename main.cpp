#include "components/error.h"
#include "components/expr.h"
#include "components/interpreter.h"
#include "components/parser.h"
#include "components/scanner.h"
#include "components/stmt.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

namespace {
static BasicErrorReporter ERROR_REPORTER;
Interpreter ip(ERROR_REPORTER);

// HACK
// We need to keep the statements parsed in the REPL because
// some of the statements (e.g. functions, classes) would be
// needed later. We can't have them destroyed when "run"
// returns.
// If we don't track, nasty bugs happen (we keep a reference to
// stmts in functions now).
static auto stmtTracker = std::make_unique<std::vector<StmtPtr>>();

void run(const std::string &source, bool trackStmt) {
  Scanner scanner(source, ERROR_REPORTER);
  auto tokens = scanner.scanTokens();
  Parser parser(tokens, ERROR_REPORTER);

  auto stmts = parser.parse();

  if (ERROR_REPORTER.hadError()) {
    return;
  }

  ip.interpret(stmts);
  if (trackStmt) {
    for (auto &&stmt : stmts) {
      stmtTracker->push_back(std::move(stmt));
    }
  }
}

void runFile(const std::string &path) {
  std::ifstream t(path);
  std::stringstream buffer;
  buffer << t.rdbuf();
  run(buffer.str(), false);
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
    try {
      run(line, true);
    } catch (std::invalid_argument *e) {
      std::cout << "invalid_argument: " << e->what() << std::endl;
    }
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
