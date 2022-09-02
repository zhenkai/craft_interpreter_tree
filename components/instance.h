#pragma once

#include "class.h"
#include <memory>

class LoxInstance {
public:
  explicit LoxInstance(LoxClass *klass) : klass(klass) {}
  std::string str() { return klass->name + " instance"; }

private:
  LoxClass *klass;
};
