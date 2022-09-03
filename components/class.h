#pragma once

#include "callable.h"
#include "function.h"
#include <string>
#include <unordered_map>

class LoxClass;
using ClassPtr = std::shared_ptr<LoxClass>;

class LoxClass : public Callable {
public:
  LoxClass(const std::string &name, ClassPtr super,
           std::unordered_map<std::string, FunPtr> methods)
      : name_(name), super_(super), methods_(std::move(methods)) {}
  std::string str() { return name_; }
  std::any call(Interpreter &ip, const std::vector<std::any> &args);
  int arity() const;
  std::string name() { return name_; }
  FunPtr findMethod(const std::string &name) const;

private:
  const std::string name_;
  ClassPtr super_;
  std::unordered_map<std::string, FunPtr> methods_;
};
