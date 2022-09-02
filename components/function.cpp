#include "function.h"

#include "env.h"
#include "instance.h"
#include "interpreter.h"
#include <memory>

std::any LoxFunction::call(Interpreter &ip, const std::vector<std::any> &args) {
  auto env = std::make_shared<Environment>(closure_);
  for (int i = 0; i < arity(); i++) {
    auto name = funDecl.params[i].lexeme;
    env->define(name, args[i]);
  }
  try {
    ip.executeBlock(funDecl.body, env);
  } catch (Return *e) {
    return e->value;
  }
  return std::any();
}

std::string LoxFunction::str() {
  return "<func name: " + funDecl.name.lexeme +
         ", arity: " + std::to_string(arity()) + ">";
}

FunPtr LoxFunction::bind(InstancePtr inst) {
  EnvPtr env = std::make_shared<Environment>(closure_);
  env->define("this", inst);
  return std::make_shared<LoxFunction>(funDecl, env);
}
