#include <string>

class ErrorReporter {
  public:
    ErrorReporter(): hadError_(false) {}
    virtual void report(int line, const std::string& where, const std::string& msg) = 0;
    bool hadError() { return hadError_; }
    void reset() { hadError_ = false; }

  protected:
    bool hadError_;
};

class BasicErrorReporter: public ErrorReporter {
  public:
    BasicErrorReporter():ErrorReporter() {}
    virtual void report(int line, const std::string& where, const std::string& msg) override;
};
