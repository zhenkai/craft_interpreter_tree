#include "class.h"
#include "instance.h"
#include <memory>

std::any LoxClass::call(Interpreter &ip, const std::vector<std::any> &args) {
  return std::make_shared<LoxInstance>(this);
}

FunPtr LoxClass::findMethod(const std::string &name) {
  if (methods_.find(name) != methods_.end()) {
    return methods_.at(name);
  }

  return nullptr;
}
