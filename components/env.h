#include <unordered_map>
#include <any>

class Environment {
public:
  Environment(): values({}) {}
  void define(const std::string& name, std::any value) { values[name] = std::move(value); }
  std::any get(const std::string& name);
private:
  std::unordered_map<std::string, std::any> values;
}
