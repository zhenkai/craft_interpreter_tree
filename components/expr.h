#pragma once

#include <any>
#include <memory>
#include "token.h"

class Visitor;

class Expr {
public:
  virtual void accept(Visitor& visitor) const = 0;
  virtual ~Expr()=default;
};

using ExprPtr = std::unique_ptr<Expr>;

class Binary: public Expr {
public:
  Binary(ExprPtr left, const Token& op, ExprPtr right): left(std::move(left)), op(op), right(std::move(right)) {}
  void accept(Visitor& visitor) const override;

  const ExprPtr left;
  const Token op;
  const ExprPtr right;
};

using BinaryPtr = std::unique_ptr<Binary>;

class Grouping: public Expr {
public:
  explicit Grouping(ExprPtr expr): expr(std::move(expr)) {}
  void accept(Visitor& visitor) const override;

  const ExprPtr expr;
};

using GroupingPtr = std::unique_ptr<Grouping>;

class Literal: public Expr {
public:
  explicit Literal(std::any value): value(std::move(value)) {}
  void accept(Visitor& visitor) const override;

  const std::any value;
};
using LiteralPtr = std::unique_ptr<Literal>;

class Unary: public Expr {
public:
  Unary(const Token& op, ExprPtr right): op(op), right(std::move(right)) {}
  void accept(Visitor& visitor) const override;

  const Token op;
  const ExprPtr right;
};
using UnaryPtr = std::unique_ptr<Unary>;

class Visitor {
public:
  virtual void visitBinaryExpr(const Binary& expr) = 0;
  virtual void visitGroupingExpr(const Grouping& expr) = 0;
  virtual void visitLiteralExpr(const Literal& expr) = 0;
  virtual void visitUnaryExpr(const Unary& expr) = 0;
};
