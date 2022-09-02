#pragma once

#include "callable.h"
#include <string>

class LoxClass : public Callable {
public:
  LoxClass(const std::string &name) : name(name) {}
  std::string str() { return name; }
  std::any call(Interpreter &ip, const std::vector<std::any> &args);
  int arity() const { return 0; }
  const std::string name;
};
