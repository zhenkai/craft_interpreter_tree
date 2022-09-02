#pragma once

#include "callable.h"
#include "env.h"
#include "stmt.h"
#include <memory>
#include <string>

class LoxFunction;
class LoxInstance;
using FunPtr = std::shared_ptr<LoxFunction>;

class LoxFunction : public Callable {
public:
  LoxFunction(const FunStmt &funDecl, bool isInitializer, EnvPtr closure)
      : funDecl(funDecl), arity_(funDecl.params.size()),
        isInitializer_(isInitializer), closure_(closure) {}
  std::any call(Interpreter &ip, const std::vector<std::any> &args) override;
  int arity() const override { return arity_; }
  FunPtr bind(std::shared_ptr<LoxInstance> inst);
  std::string str();

private:
  const FunStmt &funDecl;
  const int arity_;
  const bool isInitializer_;
  EnvPtr closure_;
};
