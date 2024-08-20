#ifndef STATEMENTS_H
#define STATEMENTS_H
#include "token.h"
#include <string>
#include <vector>

#include "expressions.h"
struct Type {
    Token token;
    bool isPointer;
};
struct Param {
    Token name;
    Type type;
};

class BlockStmt;
class VarStmt;
class FnStmt;
class ExpressionStmt;
class ReturnStmt;
class IfStmt;

class VisitStmt {
  public:
    virtual void visitBlockStmt(BlockStmt* stmt);
    virtual void visitVarStmt(VarStmt* stmt);
    virtual void visitFnStmt(FnStmt* stmt);
    virtual void visitExpressionStmt(ExpressionStmt* stmt);
    virtual void visitReturnStmt(ReturnStmt* stmt);
    virtual void visitIfStmt(IfStmt* stmt);
    virtual ~VisitStmt() = 0;
};

class Stmt {
  public:
    virtual void visit(VisitStmt visitor) = 0;
    virtual ~Stmt() = 0;
};

class BlockStmt : Stmt {
  public:
    std::vector<Stmt*> body;
    void visit(VisitStmt* visitor) {
        visitor->visitBlockStmt(this);
    }
};
class VarStmt : Stmt {
  public:
    Token name;
    Type type;
    Expr* initializer;
    void visit(VisitStmt* visitor) {
        visitor->visitVarStmt(this);
    }
};
class FnStmt : Stmt {
  public:
    Token name;
    std::vector<Param> params;
    Stmt* body;
    Type returnType;
    void visit(VisitStmt* visitor) {
        visitor->visitFnStmt(this);
    }
};
class ExpressionStmt : Stmt {
  public:
    Expr* expression;
    void visit(VisitStmt* visitor) {
        visitor->visitExpressionStmt(this);
    }
};
class ReturnStmt : Stmt {
  public:
    Expr* expression;
    void visit(VisitStmt* visitor) {
        visitor->visitReturnStmt(this);
    }
};
class IfStmt : Stmt {
  public:
    Expr* ifCondition;
    Stmt* ifBlock;
    std::vector<Expr> elifConditions;
    std::vector<Stmt> elifBlocks;
    Stmt* elseBlock;
    void visit(VisitStmt* visitor) {
        visitor->visitIfStmt(this);
    }
};

#endif
