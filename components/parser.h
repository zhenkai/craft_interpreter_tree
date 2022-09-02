#pragma once

#include "error.h"
#include "expr.h"
#include "stmt.h"
#include "token.h"
#include <exception>
#include <vector>

class ParseError : public std::exception {};

/*** Expression
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | logic_or ;
logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
arguments      → expression ( "," expression )* ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
               | IDENTIFIER ;
***/
/*** Statement
program        → declaration* EOF ;

declaration    → classDecl
               | funDecl
               | varDecl
               | statement ;

classDecl      → "class" IDENTIFIER "{" function* "}" ;

funDecl        → "fun" function ;
function       → IDENTIFIER "(" parameters? ")" block ;
parameters     → IDENTIFIER ( "," IDENTIFIER )* ;

varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;

statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | returnStmt
               | whileStmt
               | block ;

returnStmt     → "return" expression? ";" ;

forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
                 expression? ";"
                 expression? ")" statement ;

ifStmt         → "if" "(" expression ")" statement
               ( "else" statement )? ;

whileStmt      → "while" "(" expression ")" statement ;

block          → "{" declaration* "}" ;
***/
class Parser {
public:
  Parser(const std::vector<const Token> &tokens, ErrorReporter &errorReporter)
      : tokens_(tokens), current_(0), errorReporter_(errorReporter) {}
  // ExprPtr parse();
  std::vector<StmtPtr> parse();

private:
  StmtPtr declaration();
  StmtPtr statement();
  StmtPtr ifStatement();
  StmtPtr forStatement();
  StmtPtr whileStatement();
  FunStmtPtr funStatement(const std::string &name);
  StmtPtr printStatement();
  StmtPtr expressionStatement();
  StmtPtr varStatement();
  StmtPtr returnStatement();
  StmtPtr classDeclaration();
  std::vector<StmtPtr> block();
  // ----------------------------------
  bool match(const std::vector<TokenType> &&types);

  Token peek() { return tokens_[current_]; }

  Token previous() { return tokens_[current_ - 1]; }

  bool isAtEnd() { return peek().type == TokenType::EOF_; }

  Token advance() {
    if (!isAtEnd()) {
      current_++;
    }
    return previous();
  }

  bool check(TokenType type) {
    if (isAtEnd()) {
      return false;
    }

    return peek().type == type;
  }

  ExprPtr expression();
  ExprPtr assignment();
  ExprPtr equality();
  ExprPtr comparison();
  ExprPtr term();
  ExprPtr factor();
  ExprPtr unary();
  ExprPtr primary();
  ExprPtr orExpr();
  ExprPtr andExpr();
  ExprPtr call();
  ExprPtr finishCall(ExprPtr expr);
  Token consume(const TokenType type, const std::string &msg);
  ParseError *error(const Token &token, const std::string &msg);
  void synchronize();
  // ----------------------------
  const std::vector<const Token> &tokens_;
  ErrorReporter &errorReporter_;
  int current_;
};
