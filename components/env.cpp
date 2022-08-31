#include "env.h"
#include "error.h"
#include <string>

std::any Environment::get(const Token& token) {
  auto& name = token.lexeme;
  if (values_.find(name) != values_.end()) {
    return values_.at(name);
  }

  throw new RuntimeError("[Line " + std::to_string(token.line) + "] Undefined variable : " + name);
}
