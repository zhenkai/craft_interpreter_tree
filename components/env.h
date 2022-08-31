#include "token.h"
#include <any>
#include <unordered_map>

class Environment {
public:
  Environment(Environment *enclosing = nullptr)
      : enclosing_(enclosing), values_({}) {}
  void define(const std::string &name, std::any value) {
    values_[name] = std::move(value);
  }
  void assign(const Token &name, const std::any &value);
  std::any get(const Token &token) const;

private:
  Environment *enclosing_;
  std::unordered_map<std::string, std::any> values_;
};
