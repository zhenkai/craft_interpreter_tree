#pragma once

#include <string>

class RuntimeError : public std::runtime_error {
public:
  RuntimeError(const std::string &what_arg) : std::runtime_error(what_arg) {}
  RuntimeError(const char *what_arg) : std::runtime_error(what_arg) {}
};
class ErrorReporter {
public:
  ErrorReporter() : hadError_(false) {}
  virtual void report(int line, const std::string &where,
                      const std::string &msg) const = 0;
  void report(int line, const std::string &msg) const { report(line, "", msg); }
  virtual void reportRuntimeError(const RuntimeError &error) const = 0;
  bool hadError() { return hadError_; }
  bool hadRuntimeError() { return hadRuntimeError_; }
  void reset() {
    hadError_ = false;
    hadRuntimeError_ = false;
  }

protected:
  mutable bool hadError_;
  mutable bool hadRuntimeError_;
};

class BasicErrorReporter : public ErrorReporter {
public:
  BasicErrorReporter() : ErrorReporter() {}
  virtual void report(int line, const std::string &where,
                      const std::string &msg) const override;
  virtual void reportRuntimeError(const RuntimeError &error) const override;
};
