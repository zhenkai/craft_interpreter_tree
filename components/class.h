#pragma once

#include "callable.h"
#include "function.h"
#include <string>
#include <unordered_map>

class LoxClass : public Callable {
public:
  LoxClass(const std::string &name,
           std::unordered_map<std::string, FunPtr> methods)
      : name_(name), methods_(std::move(methods)) {}
  std::string str() { return name_; }
  std::any call(Interpreter &ip, const std::vector<std::any> &args);
  int arity() const;
  std::string name() { return name_; }
  FunPtr findMethod(const std::string &name) const;

private:
  const std::string name_;
  std::unordered_map<std::string, FunPtr> methods_;
};

using ClassPtr = std::shared_ptr<LoxClass>;
