#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H
#include "token.h"
#include <string>
#include <vector>

#include <llvm/IR/Value.h>

class NumberExpr;
class StringExpr;
class CharExpr;
class BoolExpr;
class IdentifierExpr;
class BinaryExpr;
class UnaryExpr;
class GroupingExpr;
class CallExpr;

template <class T> class VisitExpr {
  public:
    virtual T visitNumberExpr(NumberExpr* expr)         = 0;
    virtual T visitStringExpr(StringExpr* expr)         = 0;
    virtual T visitCharExpr(CharExpr* expr)             = 0;
    virtual T visitBoolExpr(BoolExpr* expr)             = 0;
    virtual T visitIdentifierExpr(IdentifierExpr* expr) = 0;
    virtual T visitBinaryExpr(BinaryExpr* expr)         = 0;
    virtual T visitUnaryExpr(UnaryExpr* expr)           = 0;
    virtual T visitGroupingExpr(GroupingExpr* expr)     = 0;
    virtual T visitCallExpr(CallExpr* expr)             = 0;
    virtual ~VisitExpr()                                = default;
};

class Expr {
  public:
    virtual void         visit(VisitExpr<void>* visitor)         = 0;
    virtual llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) = 0;
    virtual ~Expr()                                              = default;
};

class NumberExpr : public Expr {
  public:
    ~NumberExpr(){};
    NumberExpr() : value() {
    }
    double value;
    void   visit(VisitExpr<void>* visitor) {
        visitor->visitNumberExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitNumberExpr(this);
    }
};
class StringExpr : public Expr {
  public:
    ~StringExpr(){};
    StringExpr() : value() {
    }
    std::string value;
    void        visit(VisitExpr<void>* visitor) {
        visitor->visitStringExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitStringExpr(this);
    }
};
class CharExpr : public Expr {
  public:
    ~CharExpr(){};
    CharExpr() : value() {
    }
    std::string value;
    void        visit(VisitExpr<void>* visitor) {
        visitor->visitCharExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitCharExpr(this);
    }
};
class BoolExpr : public Expr {
  public:
    ~BoolExpr(){};
    BoolExpr() : value() {
    }
    bool value;
    void visit(VisitExpr<void>* visitor) {
        visitor->visitBoolExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitBoolExpr(this);
    }
};
class IdentifierExpr : public Expr {
  public:
    ~IdentifierExpr(){};
    IdentifierExpr() : value() {
    }
    Token value;
    void  visit(VisitExpr<void>* visitor) {
        visitor->visitIdentifierExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitIdentifierExpr(this);
    }
};
class BinaryExpr : public Expr {
  public:
    ~BinaryExpr(){};
    BinaryExpr() : left(nullptr), oper(), right(nullptr) {
    }
    std::unique_ptr<Expr> left;
    Token                 oper;
    std::unique_ptr<Expr> right;
    void                  visit(VisitExpr<void>* visitor) {
        visitor->visitBinaryExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitBinaryExpr(this);
    }
};
class UnaryExpr : public Expr {
  public:
    ~UnaryExpr(){};
    UnaryExpr() : oper(), right(nullptr) {
    }
    Token                 oper;
    std::unique_ptr<Expr> right;
    void                  visit(VisitExpr<void>* visitor) {
        visitor->visitUnaryExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitUnaryExpr(this);
    }
};
class GroupingExpr : public Expr {
  public:
    ~GroupingExpr(){};
    GroupingExpr() : expression(nullptr) {
    }
    std::unique_ptr<Expr> expression;
    void                  visit(VisitExpr<void>* visitor) {
        visitor->visitGroupingExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitGroupingExpr(this);
    }
};
class CallExpr : public Expr {
  public:
    ~CallExpr(){};
    CallExpr() : callee(nullptr), args() {
    }
    std::unique_ptr<Expr> callee;
    std::vector<Expr>     args;
    void                  visit(VisitExpr<void>* visitor) {
        visitor->visitCallExpr(this);
    }
    llvm::Value* visit(VisitExpr<llvm::Value*>* visitor) {
        return visitor->visitCallExpr(this);
    }
};

#endif
