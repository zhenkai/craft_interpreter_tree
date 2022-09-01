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
  std::any get(const Token &token) const;

private:
  EnvPtr enclosing_;
  std::unordered_map<std::string, std::any> values_;
};
