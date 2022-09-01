#pragma once

#include "callable.h"
#include "stmt.h"
#include <string>

class LoxFunction : public Callable {
public:
  LoxFunction(const FunStmt &funDecl)
      : funDecl(funDecl), arity_(funDecl.params.size()) {}
  std::any call(Interpreter &ip, const std::vector<std::any> &args) override;
  int arity() const override { return arity_; }
  std::string str();

private:
  const FunStmt &funDecl;
  const int arity_;
};
