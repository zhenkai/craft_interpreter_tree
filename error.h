#pragma once

#include <string>

class ErrorReporter {
  public:
    ErrorReporter(): hadError_(false) {}
    virtual void report(int line, const std::string& where, const std::string& msg) const = 0;
    void report(int line, const std::string& msg) const { report(line, "", msg); }
    bool hadError() { return hadError_; }
    void reset() { hadError_ = false; }

  protected:
    mutable bool hadError_;
};

class BasicErrorReporter: public ErrorReporter {
  public:
    BasicErrorReporter():ErrorReporter() {}
    virtual void report(int line, const std::string& where, const std::string& msg) const override;
};
