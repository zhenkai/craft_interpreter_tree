#include "class.h"
#include "instance.h"
#include <memory>

std::any LoxClass::call(Interpreter &ip, const std::vector<std::any> &args) {
  return std::make_shared<LoxInstance>(this);
}
