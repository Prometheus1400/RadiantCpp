#ifndef STATEMENTS_H
#define STATEMENTS_H
#include "token.h"
#include <string>
#include <vector>

#include "expressions.h"
struct Type {
    Token token;
    bool  isPointer;
};
struct Param {
    Token name;
    Type  type;
};

class BlockStmt;
class VarStmt;
class FnStmt;
class ExpressionStmt;
class ReturnStmt;
class IfStmt;

class VisitStmt {
  public:
    virtual void visitBlockStmt(BlockStmt* stmt)           = 0;
    virtual void visitVarStmt(VarStmt* stmt)               = 0;
    virtual void visitFnStmt(FnStmt* stmt)                 = 0;
    virtual void visitExpressionStmt(ExpressionStmt* stmt) = 0;
    virtual void visitReturnStmt(ReturnStmt* stmt)         = 0;
    virtual void visitIfStmt(IfStmt* stmt)                 = 0;
    virtual ~VisitStmt()                                   = default;
};

class Stmt {
  public:
    virtual void visit(VisitStmt* visitor) = 0;
    virtual ~Stmt()                        = default;
};

class BlockStmt : public Stmt {
  public:
    ~BlockStmt(){};
    BlockStmt() : body() {
    }
    std::vector<std::unique_ptr<Stmt>> body;
    void                               visit(VisitStmt* visitor) {
        visitor->visitBlockStmt(this);
    }
};
class VarStmt : public Stmt {
  public:
    ~VarStmt(){};
    VarStmt() : name(), type(), initializer(nullptr) {
    }
    Token                 name;
    Type                  type;
    std::unique_ptr<Expr> initializer;
    void                  visit(VisitStmt* visitor) {
        visitor->visitVarStmt(this);
    }
};
class FnStmt : public Stmt {
  public:
    ~FnStmt(){};
    FnStmt() : name(), params(), body(nullptr), returnType() {
    }
    Token                 name;
    std::vector<Param>    params;
    std::unique_ptr<Stmt> body;
    Type                  returnType;
    void                  visit(VisitStmt* visitor) {
        visitor->visitFnStmt(this);
    }
};
class ExpressionStmt : public Stmt {
  public:
    ~ExpressionStmt(){};
    ExpressionStmt() : expression(nullptr) {
    }
    std::unique_ptr<Expr> expression;
    void                  visit(VisitStmt* visitor) {
        visitor->visitExpressionStmt(this);
    }
};
class ReturnStmt : public Stmt {
  public:
    ~ReturnStmt(){};
    ReturnStmt() : expression(nullptr) {
    }
    std::unique_ptr<Expr> expression;
    void                  visit(VisitStmt* visitor) {
        visitor->visitReturnStmt(this);
    }
};
class IfStmt : public Stmt {
  public:
    ~IfStmt(){};
    IfStmt() : ifCondition(nullptr), ifBlock(nullptr), elifConditions(), elifBlocks(), elseBlock(nullptr) {
    }
    std::unique_ptr<Expr> ifCondition;
    std::unique_ptr<Stmt> ifBlock;
    std::vector<Expr>     elifConditions;
    std::vector<Stmt>     elifBlocks;
    std::unique_ptr<Stmt> elseBlock;
    void                  visit(VisitStmt* visitor) {
        visitor->visitIfStmt(this);
    }
};

#endif
