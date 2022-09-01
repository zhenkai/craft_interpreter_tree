#include "native.h"
#include <chrono>

std::any LoxClock::call(Interpreter &ip, const std::vector<std::any> &args) {
  const auto now = std::chrono::system_clock::now();
  return static_cast<int>(
      std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())
          .count());
}
