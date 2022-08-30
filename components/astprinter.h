#include "expr.h"
#include <sstream>

class AstPrinter: public Visitor {
public:
  VisitorResT visitBinaryExpr(const Binary& expr) override;
  VisitorResT visitGroupingExpr(const Grouping& expr) override;
  VisitorResT visitLiteralExpr(const Literal& expr) override;
  VisitorResT visitUnaryExpr(const Unary& expr) override;
  std::string print(const Expr& expr);
private:
  void parenthesize(const std::string& name, const std::vector<const Expr*>& exprs);
  std::stringstream s_;
};
