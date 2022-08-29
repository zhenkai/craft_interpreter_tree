#pragma once

#include <any>
#include "token.h"

template <typename T> class Visitor;

class Expr {
  tempate <typename T> T accept(Visitor<T>& visitor) = 0;
};

class Binary: public Expr {
public:
  Binary(const Expr& left, const Token& op, const Expr& right): left(left), op(op), right(right) {}
  template <typename T> accept(Visitor<T>& visitor) override {
    return visitor.visitBinaryExpr(this);
  }

  const Expr left;
  const Token op;
  const Expr right;
};

class Grouping: public Expr {
public:
  explicit Grouping(const Expr& expr): expr(expr) {}
  template <typename T> accept(Visitor<T>& visitor) override {
    return visitor.visitGroupingExpr(this);
  }

  const Expr expr;
};

class Literal: public Expr {
public:
  explicit Literal(const std::any& value): value(value) {}
  template <typename T> accept(Visitor<T>& visitor) override {
    return visitor.visitLiteralExpr(this);
  }

  const std::any value;
};

class Unary: public Expr {
public:
  Unary(const Token& op, const Expr& right): op(op), right(right) {}
  template <typename T> accept(Visitor<T>& visitor) override {
    return visitor.visitUnaryExpr(this);
  }

  const Token op;
  const Expr right;
};

template <typename T> class Visitor {
  T visitBinaryExpr(const Binary& expr) = 0;
  T visitGroupExpr(const Grouping& expr) = 0;
  T visitLiteralExpr(const Literal& expr) = 0;
  T visitUnaryExpr(const Unary& expr) = 0;
};
