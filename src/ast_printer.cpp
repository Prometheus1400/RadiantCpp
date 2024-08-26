#include "include/ast_printer.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::unique_ptr;
using std::stringstream;

vector<string> ASTPrinter::print(const vector<unique_ptr<Stmt>>& stmts) {
    vector<string> lines;
    for (const auto& stmt : stmts) {
        this->ss = stringstream();
        stmt->visit(this);
        lines.push_back(this->ss.str());
    }
    return lines;
}

void ASTPrinter::visitBlockStmt(BlockStmt* stmt) {}
void ASTPrinter::visitVarStmt(VarStmt* stmt) {}
void ASTPrinter::visitFnStmt(FnStmt* stmt) {}
void ASTPrinter::visitExpressionStmt(ExpressionStmt* stmt) {
    stmt->expression->visit(this);
}
void ASTPrinter::visitReturnStmt(ReturnStmt* stmt) {}
void ASTPrinter::visitIfStmt(IfStmt* stmt) {}

void ASTPrinter::visitNumberExpr(NumberExpr* expr) {
    this->ss << expr->value;
}
void ASTPrinter::visitStringExpr(StringExpr* expr) {}
void ASTPrinter::visitCharExpr(CharExpr* expr) {}
void ASTPrinter::visitBoolExpr(BoolExpr* expr) {}
void ASTPrinter::visitIdentifierExpr(IdentifierExpr* expr) {}
void ASTPrinter::visitBinaryExpr(BinaryExpr* expr) {
    this->ss << "(";
    expr->left->visit(this);
    this->ss << expr->oper.lexeme;
    expr->right->visit(this);
    this->ss << ")";
}
void ASTPrinter::visitUnaryExpr(UnaryExpr* expr) {
    this->ss << expr->oper.lexeme;
    expr->right->visit(this);
}
void ASTPrinter::visitGroupingExpr(GroupingExpr* expr) {
    /* this->ss << "("; */
    expr->expression->visit(this);
    /* this->ss << ")"; */
}
void ASTPrinter::visitCallExpr(CallExpr* expr) {}
