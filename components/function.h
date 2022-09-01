#pragma once

#include "callable.h"
#include "stmt.h"

class LoxFunction : Callable {
public:
  LoxFunction(const FunStmt &funDecl) : funDecl(funDecl) {}
  std::any call(Interpreter &ip, const std::vector<std::any> &args) override;
  int arity() const override;

private:
  const FunStmt &funDecl;
};
