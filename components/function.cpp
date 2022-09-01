#include "function.h"

#include "env.h"
#include "interpreter.h"
#include <memory>

std::any LoxFunction::call(Interpreter &ip, const std::vector<std::any> &args) {
  auto env = std::make_shared<Environment>(ip.globalEnv());
  for (int i = 0; i < arity(); i++) {
    auto name = funDecl.params[i].lexeme;
    env->define(name, args[i]);
  }

  ip.executeBlock(funDecl.body, env);
  return std::any();
}

std::string LoxFunction::str() {
  return "<func name: " + funDecl.name.lexeme +
         ", arity: " + std::to_string(arity()) + ">";
}
