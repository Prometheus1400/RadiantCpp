#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "expressions.h"
#include "statements.h"
#include <memory>
#include <vector>
#include <sstream>

class ASTPrinter : public VisitStmt, public VisitExpr {
  public:
    ASTPrinter() {}
    /* ~ASTPrinter() {}; */
    std::vector<std::string> print(const std::vector<std::unique_ptr<Stmt>>& stmts);
    void visitBlockStmt(BlockStmt* stmt) override;
    void visitVarStmt(VarStmt* stmt) override;
    void visitFnStmt(FnStmt* stmt) override;
    void visitExpressionStmt(ExpressionStmt* stmt) override;
    void visitReturnStmt(ReturnStmt* stmt) override;
    void visitIfStmt(IfStmt* stmt) override;

    void visitNumberExpr(NumberExpr* expr) override;
    void visitStringExpr(StringExpr* expr) override;
    void visitCharExpr(CharExpr* expr) override;
    void visitBoolExpr(BoolExpr* expr) override;
    void visitIdentifierExpr(IdentifierExpr* expr) override;
    void visitBinaryExpr(BinaryExpr* expr) override;
    void visitUnaryExpr(UnaryExpr* expr) override;
    void visitGroupingExpr(GroupingExpr* expr) override;
    void visitCallExpr(CallExpr* expr) override;
  private:
    std::stringstream ss;
};

#endif
