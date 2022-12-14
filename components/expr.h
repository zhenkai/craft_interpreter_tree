#pragma once

#include "token.h"
#include <any>
#include <memory>
#include <vector>

using ExprVisitorResT = std::any;

class ExprVisitor;

class Expr {
public:
  virtual ExprVisitorResT accept(ExprVisitor &visitor) const = 0;
  virtual ~Expr() = default;
};

using ExprPtr = std::unique_ptr<Expr>;

class Binary : public Expr {
public:
  Binary(ExprPtr left, const Token &op, ExprPtr right)
      : left(std::move(left)), op(op), right(std::move(right)) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const ExprPtr left;
  const Token op;
  const ExprPtr right;
};

using BinaryPtr = std::unique_ptr<Binary>;

class Grouping : public Expr {
public:
  explicit Grouping(ExprPtr expr) : expr(std::move(expr)) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const ExprPtr expr;
};

using GroupingPtr = std::unique_ptr<Grouping>;

class Literal : public Expr {
public:
  explicit Literal(std::any value) : value(std::move(value)) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const std::any value;
};
using LiteralPtr = std::unique_ptr<Literal>;

class Unary : public Expr {
public:
  Unary(const Token &op, ExprPtr right) : op(op), right(std::move(right)) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const Token op;
  const ExprPtr right;
};
using UnaryPtr = std::unique_ptr<Unary>;

class Variable : public Expr {
public:
  Variable(const Token &name) : name(name) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const Token name;
};
using VariablePtr = std::unique_ptr<Variable>;

class Assignment : public Expr {
public:
  Assignment(const Token &name, ExprPtr value)
      : name(name), value(std::move(value)) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const Token name;
  const ExprPtr value;
};
using AssignmentPtr = std::unique_ptr<Assignment>;

class Logical : public Expr {
public:
  Logical(const Token &op, ExprPtr left, ExprPtr right)
      : op(op), left(std::move(left)), right(std::move(right)) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const Token op;
  const ExprPtr left;
  const ExprPtr right;
};

using LogicalPtr = std::unique_ptr<Logical>;

class Call : public Expr {
public:
  Call(ExprPtr callee, const Token &paren, std::vector<ExprPtr> arguments)
      : callee(std::move(callee)), paren(paren),
        arguments(std::move(arguments)) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const ExprPtr callee;
  const Token paren;
  const std::vector<ExprPtr> arguments;
};

using CallPtr = std::unique_ptr<Call>;

class Get : public Expr {
public:
  Get(ExprPtr object, const Token &name)
      : object(std::move(object)), name(name) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  // HACK: not const as we may need to move it to Set
  // during parsing.
  ExprPtr object;
  const Token name;
};

using GetPtr = std::unique_ptr<Get>;

class Set : public Expr {
public:
  Set(ExprPtr object, const Token &name, ExprPtr value)
      : object(std::move(object)), name(name), value(std::move(value)) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const ExprPtr object;
  const Token name;
  const ExprPtr value;
};

using SetPtr = std::unique_ptr<Set>;

class This : public Expr {
public:
  This(const Token &keyword) : keyword(keyword) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const Token keyword;
};

using ThisPtr = std::unique_ptr<This>;

class Super : public Expr {
public:
  Super(const Token &keyword, const Token &method)
      : keyword(keyword), method(method) {}
  ExprVisitorResT accept(ExprVisitor &visitor) const override;

  const Token keyword;
  const Token method;
};

using SuperPtr = std::unique_ptr<Super>;

class ExprVisitor {
public:
  virtual ExprVisitorResT visitBinaryExpr(const Binary &expr) = 0;
  virtual ExprVisitorResT visitGroupingExpr(const Grouping &expr) = 0;
  virtual ExprVisitorResT visitLiteralExpr(const Literal &expr) = 0;
  virtual ExprVisitorResT visitUnaryExpr(const Unary &expr) = 0;
  virtual ExprVisitorResT visitVariableExpr(const Variable &expr) = 0;
  virtual ExprVisitorResT visitAssignmentExpr(const Assignment &expr) = 0;
  virtual ExprVisitorResT visitLogicalExpr(const Logical &expr) = 0;
  virtual ExprVisitorResT visitCallExpr(const Call &expr) = 0;
  virtual ExprVisitorResT visitGetExpr(const Get &expr) = 0;
  virtual ExprVisitorResT visitSetExpr(const Set &expr) = 0;
  virtual ExprVisitorResT visitThisExpr(const This &expr) = 0;
  virtual ExprVisitorResT visitSuperExpr(const Super &expr) = 0;
  virtual ~ExprVisitor() = default;
};
