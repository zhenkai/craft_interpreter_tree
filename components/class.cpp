#include "class.h"
#include "instance.h"
#include <memory>

std::any LoxClass::call(Interpreter &ip, const std::vector<std::any> &args) {
  auto instance = std::make_shared<LoxInstance>(this);
  FunPtr initializer = findMethod("init");
  if (initializer != nullptr) {
    initializer->bind(instance)->call(ip, args);
  }
  return instance;
}

FunPtr LoxClass::findMethod(const std::string &name) const {
  if (methods_.find(name) != methods_.end()) {
    return methods_.at(name);
  }

  return nullptr;
}

int LoxClass::arity() const {
  FunPtr initializer = findMethod("init");
  if (initializer != nullptr) {
    return initializer->arity();
  }
  return 0;
}
