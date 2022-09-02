#pragma once

#include "token.h"
#include <any>
#include <memory>
#include <unordered_map>

class Environment;
using EnvPtr = std::shared_ptr<Environment>;

class Environment {
public:
  Environment(EnvPtr enclosing = nullptr)
      : enclosing_(enclosing), values_({}) {}
  void define(const std::string &name, std::any value) {
    values_[name] = std::move(value);
  }
  void assign(const Token &name, const std::any &value);
  void assignAt(int dist, const Token &name, const std::any &value);
  std::any get(const Token &token);
  std::any getAt(int dist, const std::string &name);

private:
  Environment *ancestor(int dist);

  EnvPtr enclosing_;
  std::unordered_map<std::string, std::any> values_;
};
