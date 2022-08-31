#include "token.h"
#include <unordered_map>
#include <any>

class Environment {
public:
  Environment(): values_({}) {}
  void define(const std::string& name, std::any value) { values_[name] = std::move(value); }
  std::any get(const Token& token);
private:
  std::unordered_map<std::string, std::any> values_;
};
