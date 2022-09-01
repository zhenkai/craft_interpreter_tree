#pragma once

#include <any>
#include <memory>

class Interpreter;

class Callable {
public:
  virtual std::any call(Interpreter &ip, const std::vector<std::any> &args) = 0;
  virtual int arity() const = 0;
  virtual ~Callable() = default;
};

using CallablePtr = std::shared_ptr<Callable>;
