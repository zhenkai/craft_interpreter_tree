#include "expr.h"

class AstPrinter: public Visitor<std::string> {
public:
  std::string visitBinaryExpr(const Binary& expr);
  std::string visitGroupExpr(const Grouping& expr);
  std::string visitLiteralExpr(const Literal& expr);
  std::string visitUnaryExpr(const Unary& expr);
  std::string print(const Expr& expr);
private:
  std::string AstPrinter::parenthesize(const std::string& name, const std::vector<Expr>& exprs);
}
