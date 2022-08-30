#pragma once

#include "expr.h"
#include <memory>

using StmtVisitorResT = void;

class StmtVisitor;

class Stmt {
public:
  virtual StmtVisitorResT accept(StmtVisitor& visitor) const = 0;
  virtual ~Stmt()=default;
};

using StmtPtr = std::unique_ptr<Stmt>;

class ExpressionStmt: public Stmt {
public:
  ExpressionStmt(ExprPtr expr): expr(std::move(expr)) {}
  StmtVisitorResT accept(StmtVisitor& visitor) const override;

  const ExprPtr expr;
};

using ExpressionStmtPtr = std::unique_ptr<ExpressionStmt>;

class PrintStmt: public Stmt {
public:
  PrintStmt(ExprPtr expr): expr(std::move(expr)) {}
  StmtVisitorResT accept(StmtVisitor& visitor) const override;

  const ExprPtr expr;
};

using PrintStmtPtr = std::unique_ptr<PrintStmt>;

class StmtVisitor {
public:
  virtual StmtVisitorResT visitExpressionStmt(const ExpressionStmt& stmt) = 0;
  virtual StmtVisitorResT visitPrintStmt(const PrintStmt& stmt) = 0;
  virtual ~StmtVisitor()=default;
};
