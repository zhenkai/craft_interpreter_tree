#pragma once

#include "expr.h"
#include "token.h"
#include <memory>
#include <vector>

using StmtVisitorResT = void;

class StmtVisitor;

class Stmt {
public:
  virtual StmtVisitorResT accept(StmtVisitor &visitor) const = 0;
  virtual ~Stmt() = default;
};

using StmtPtr = std::unique_ptr<Stmt>;

class ExpressionStmt : public Stmt {
public:
  ExpressionStmt(ExprPtr expr) : expr(std::move(expr)) {}
  StmtVisitorResT accept(StmtVisitor &visitor) const override;

  const ExprPtr expr;
};

using ExpressionStmtPtr = std::unique_ptr<ExpressionStmt>;

class PrintStmt : public Stmt {
public:
  PrintStmt(ExprPtr expr) : expr(std::move(expr)) {}
  StmtVisitorResT accept(StmtVisitor &visitor) const override;

  const ExprPtr expr;
};

using PrintStmtPtr = std::unique_ptr<PrintStmt>;

class VarDecl : public Stmt {
public:
  VarDecl(const Token &name, ExprPtr expr)
      : name(name), initializer(std::move(expr)) {}
  StmtVisitorResT accept(StmtVisitor &visitor) const override;

  const Token name;
  const ExprPtr initializer;
};

using VarDeclPtr = std::unique_ptr<VarDecl>;

class Block : public Stmt {
public:
  Block(std::vector<StmtPtr> stmts) : stmts(std::move(stmts)) {}
  StmtVisitorResT accept(StmtVisitor &visitor) const override;

  const std::vector<StmtPtr> stmts;
};

class StmtVisitor {
public:
  virtual StmtVisitorResT visitExpressionStmt(const ExpressionStmt &stmt) = 0;
  virtual StmtVisitorResT visitPrintStmt(const PrintStmt &stmt) = 0;
  virtual StmtVisitorResT visitVarDecl(const VarDecl &stmt) = 0;
  virtual StmtVisitorResT visitBlock(const Block &block) = 0;
  virtual ~StmtVisitor() = default;
};
