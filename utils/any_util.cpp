#include "any_util.h"
#include "../components/class.h"
#include "../components/function.h"
#include "../components/instance.h"
#include "../components/native.h"
#include <cmath>
#include <limits>
#include <memory>
#include <sstream>

std::string anyToStr(const std::any &a) {
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
    } else if (a.type() == typeid(std::shared_ptr<LoxFunction>)) {
      s << std::any_cast<std::shared_ptr<LoxFunction>>(a)->str() << std::endl;
    } else if (a.type() == typeid(std::shared_ptr<LoxClass>)) {
      s << std::any_cast<std::shared_ptr<LoxClass>>(a)->str() << std::endl;
    } else if (a.type() == typeid(std::shared_ptr<LoxInstance>)) {
      s << std::any_cast<std::shared_ptr<LoxInstance>>(a)->str() << std::endl;
    } else if (a.type() == typeid(std::shared_ptr<LoxClock>)) {
      s << "<Native function: clock>" << std::endl;
    } else {
      throw new std::invalid_argument(a.type().name());
    }
    return s.str();
  }
  return "Nil";
}

bool anyEqual(const std::any &a, const std::any &b) {
  if (!a.has_value()) {
    return !b.has_value();
  }
  if (!b.has_value()) {
    return !a.has_value();
  }

  if (a.type() != b.type()) {
    return false;
  }

  if (a.type() == typeid(std::string)) {
    return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
  } else if (a.type() == typeid(double)) {
    return std::fabs(std::any_cast<double>(a) - std::any_cast<double>(b)) <
           std::numeric_limits<double>::epsilon();
  } else if (a.type() == typeid(int)) {
    return std::any_cast<int>(a) == std::any_cast<int>(b);
  } else if (a.type() == typeid(float)) {
    return std::fabs(std::any_cast<float>(a) - std::any_cast<float>(b)) <
           std::numeric_limits<float>::epsilon();
  } else if (a.type() == typeid(bool)) {
    return std::any_cast<bool>(a) == std::any_cast<bool>(b);
  }
  throw new std::invalid_argument("unsupported type for comparison");
}
