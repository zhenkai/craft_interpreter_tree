#include "env.h"
#include "error.h"
#include <string>

std::any Environment::get(const Token &token) const {
  auto &name = token.lexeme;
  if (values_.find(name) != values_.end()) {
    return values_.at(name);
  }

  if (enclosing_ != nullptr) {
    return enclosing_->get(token);
  }

  throw new RuntimeError("[Line " + std::to_string(token.line) +
                         "] Undefined variable : " + name);
}

void Environment::assign(const Token &name, const std::any &value) {
  if (values_.find(name.lexeme) != values_.end()) {
    values_[name.lexeme] = value;
    return;
  }
  if (enclosing_ != nullptr) {
    enclosing_->assign(name, value);
  }
  throw new RuntimeError("[Line " + std::to_string(name.line) +
                         "] Undefined variable : " + name.lexeme);
}
