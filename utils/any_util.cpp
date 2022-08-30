#include "any_util.h"
#include <sstream>

std::string anyToStr(const std::any& a) {
  std::stringstream s;
  if (a.has_value()) {
    if (a.type() == typeid(std::string)) {
      s << std::any_cast<std::string>(a);
    } else if (a.type() == typeid(double)) {
      s << std::any_cast<double>(a);
    } else if (a.type() == typeid(int)) {
      s << std::any_cast<int>(a);
    } else if (a.type() == typeid(float)) {
      s << std::any_cast<float>(a);
    } else if (a.type() == typeid(bool)) {
      s << (std::any_cast<bool>(a) ? "true" : "false");
    } else {
      s << "LITERAL: " << a.type().name();
    }
    return s.str();
  }
  return "Nil";
}
