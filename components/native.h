#include "callable.h"

/**
 * Lox functions that are not implemented with Lox itself,
 * but rather implemented by underlying platform (c++).
 *
 * These functions are to be exposed to Lox users via interpreter
 * by being injected into the global environment at startup.
 **/

class LoxClock : public Callable {
public:
  // returns seconds since epoch
  std::any call(Interpreter &ip, const std::vector<std::any> &args) override;
  int arity() const override { return 0; };
};
