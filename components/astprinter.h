#include "expr.h"
#include <sstream>

class AstPrinter: public Visitor {
public:
  void visitBinaryExpr(const Binary& expr) override;
  void visitGroupingExpr(const Grouping& expr) override;
  void visitLiteralExpr(const Literal& expr) override;
  void visitUnaryExpr(const Unary& expr) override;
  std::string print(const Expr& expr);
private:
  void parenthesize(const std::string& name, const std::vector<const Expr*>& exprs);
  std::stringstream s_;
};
