#pragma once

#include "error.h"
#include "expr.h"
#include "stmt.h"
#include <unordered_map>
#include <vector>

// Value meaning:
// false: variable declared but not defined (i.e. initializer not ran)
// true: variable defined
using SymbolMap = std::unordered_map<std::string, bool>;

enum class FunctionType { NONE, FUNCTION, METHOD, INIT };
enum class ClassType { NONE, CLASS };

class Interpreter;
class Resolver : public ExprVisitor, public StmtVisitor {
public:
  explicit Resolver(Interpreter &ip, ErrorReporter &errorReporter);
  ExprVisitorResT visitBinaryExpr(const Binary &expr) override;
  ExprVisitorResT visitGroupingExpr(const Grouping &expr) override;
  ExprVisitorResT visitLiteralExpr(const Literal &expr) override;
  ExprVisitorResT visitUnaryExpr(const Unary &expr) override;
  ExprVisitorResT visitVariableExpr(const Variable &expr) override;
  ExprVisitorResT visitAssignmentExpr(const Assignment &expr) override;
  ExprVisitorResT visitLogicalExpr(const Logical &expr) override;
  ExprVisitorResT visitCallExpr(const Call &expr) override;
  ExprVisitorResT visitGetExpr(const Get &expr) override;
  ExprVisitorResT visitSetExpr(const Set &expr) override;
  ExprVisitorResT visitThisExpr(const This &expr) override;
  ExprVisitorResT visitSuperExpr(const Super &expr) override;
  StmtVisitorResT visitPrintStmt(const PrintStmt &stmt) override;
  StmtVisitorResT visitExpressionStmt(const ExpressionStmt &stmt) override;
  StmtVisitorResT visitVarDecl(const VarDecl &stmt) override;
  StmtVisitorResT visitBlock(const Block &block) override;
  StmtVisitorResT visitIfStmt(const IfStmt &stmt) override;
  StmtVisitorResT visitWhileStmt(const WhileStmt &stmt) override;
  StmtVisitorResT visitFunStmt(const FunStmt &stmt) override;
  StmtVisitorResT visitReturnStmt(const ReturnStmt &stmt) override;
  StmtVisitorResT visitClassStmt(const ClassStmt &stmt) override;
  void resolve(const std::vector<StmtPtr> &stmts);

private:
  void resolve(const StmtPtr &stmt);
  void resolve(const Stmt &stmt);
  void resolve(const Expr &expr);
  void resolve(const ExprPtr &expr);
  void resolveLocal(const Expr &expr, const Token &name);
  void resolveFun(const FunStmt &fun, FunctionType type);
  void beginScope();
  void endScope();
  void declare(const Token &name);
  void define(const Token &name);

  Interpreter &ip_;
  ErrorReporter &errorReporter_;
  std::vector<SymbolMap> scopes_;
  FunctionType currentFunction_;
  ClassType currentClass_;
};
