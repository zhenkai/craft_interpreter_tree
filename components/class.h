#include <string>

class LoxClass {
public:
  LoxClass(const std::string &name) : name(name) {}
  std::string str() { return name; }
  const std::string name;
};
