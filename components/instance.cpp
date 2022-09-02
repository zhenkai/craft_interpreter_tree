#include "instance.h"
#include "error.h"

std::any LoxInstance::get(const Token &name) {
  if (fields_.find(name.lexeme) != fields_.end()) {
    return fields_.at(name.lexeme);
  }

  FunPtr method = klass_->findMethod(name.lexeme);
  if (method != nullptr) {
    return method->bind(shared_from_this());
  }

  throw new RuntimeError(name.errorStr() + ". Undefined property '" +
                         name.lexeme + "'.");
}
