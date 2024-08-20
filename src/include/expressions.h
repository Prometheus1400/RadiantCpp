#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H
#include "token.h"
#include <string>
#include <vector>

class NumberExpr;
class StringExpr;
class CharExpr;
class BoolExpr;
class IdentifierExpr;
class BinaryExpr;
class UnaryExpr;
class GroupingExpr;
class CallExpr;

class VisitExpr {
  public:
    virtual void visitNumberExpr(NumberExpr* expr);
    virtual void visitStringExpr(StringExpr* expr);
    virtual void visitCharExpr(CharExpr* expr);
    virtual void visitBoolExpr(BoolExpr* expr);
    virtual void visitIdentifierExpr(IdentifierExpr* expr);
    virtual void visitBinaryExpr(BinaryExpr* expr);
    virtual void visitUnaryExpr(UnaryExpr* expr);
    virtual void visitGroupingExpr(GroupingExpr* expr);
    virtual void visitCallExpr(CallExpr* expr);
    virtual ~VisitExpr() = 0;
};

class Expr {
  public:
    virtual void visit(VisitExpr visitor) = 0;
    virtual ~Expr() = 0;
};

class NumberExpr : Expr {
  public:
    std::string value;
    void visit(VisitExpr* visitor) {
        visitor->visitNumberExpr(this);
    }
};
class StringExpr : Expr {
  public:
    std::string value;
    void visit(VisitExpr* visitor) {
        visitor->visitStringExpr(this);
    }
};
class CharExpr : Expr {
  public:
    std::string value;
    void visit(VisitExpr* visitor) {
        visitor->visitCharExpr(this);
    }
};
class BoolExpr : Expr {
  public:
    std::string value;
    void visit(VisitExpr* visitor) {
        visitor->visitBoolExpr(this);
    }
};
class IdentifierExpr : Expr {
  public:
    Token value;
    void visit(VisitExpr* visitor) {
        visitor->visitIdentifierExpr(this);
    }
};
class BinaryExpr : Expr {
  public:
    std::unique_ptr<Expr> left;
    Token oper;
    std::unique_ptr<Expr> right;
    void visit(VisitExpr* visitor) {
        visitor->visitBinaryExpr(this);
    }
};
class UnaryExpr : Expr {
  public:
    Token oper;
    std::unique_ptr<Expr> right;
    void visit(VisitExpr* visitor) {
        visitor->visitUnaryExpr(this);
    }
};
class GroupingExpr : Expr {
  public:
    std::unique_ptr<Expr> expression;
    void visit(VisitExpr* visitor) {
        visitor->visitGroupingExpr(this);
    }
};
class CallExpr : Expr {
  public:
    std::unique_ptr<Expr> callee;
    std::vector<Expr> args;
    void visit(VisitExpr* visitor) {
        visitor->visitCallExpr(this);
    }
};

#endif
