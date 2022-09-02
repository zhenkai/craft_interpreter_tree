#pragma once

#include "class.h"
#include "token.h"
#include <memory>
#include <unordered_map>

class LoxInstance {
public:
  explicit LoxInstance(LoxClass *klass)
      : klass_(klass), fields_(std::unordered_map<std::string, std::any>()) {}
  std::any get(const Token &name);
  std::string str() { return klass_->name + " instance"; }

private:
  LoxClass *klass_;
  std::unordered_map<std::string, std::any> fields_;
};

using InstancePtr = std::shared_ptr<LoxInstance>;
