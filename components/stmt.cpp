#include "stmt.h"

StmtVisitorResT ExpressionStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitExpressionStmt(*this);
}

StmtVisitorResT PrintStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitPrintStmt(*this);
}

StmtVisitorResT VarDecl::accept(StmtVisitor &visitor) const {
  return visitor.visitVarDecl(*this);
}

StmtVisitorResT Block::accept(StmtVisitor &visitor) const {
  return visitor.visitBlock(*this);
}

StmtVisitorResT IfStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitIfStmt(*this);
}

StmtVisitorResT WhileStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitWhileStmt(*this);
}

StmtVisitorResT FunStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitFunStmt(*this);
}

StmtVisitorResT ReturnStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitReturnStmt(*this);
}

StmtVisitorResT ClassStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitClassStmt(*this);
}
