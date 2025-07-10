#include "include/ast_printer.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

vector<string> ASTPrinter::print(const vector<unique_ptr<Stmt>>& stmts) {
    vector<string> lines;
    for (const auto& stmt : stmts) {
        this->ss = stringstream();
        stmt->visit(this);
        lines.push_back(this->ss.str());
    }
    return lines;
}

void ASTPrinter::visitBlockStmt(BlockStmt* stmt) {
}
void ASTPrinter::visitVarStmt(VarStmt* stmt) {
    this->ss << "let " << stmt->name.lexeme << ":" << stmt->type.token.lexeme << ":" << stmt->type.isPointer << ":" << stmt->type.isArray << " = ";
    stmt->initializer->visit(this);
}
void ASTPrinter::visitFnStmt(FnStmt* stmt) {
}
void ASTPrinter::visitExpressionStmt(ExpressionStmt* stmt) {
    stmt->expression->visit(this);
}
void ASTPrinter::visitReturnStmt(ReturnStmt* stmt) {
}
void ASTPrinter::visitIfStmt(IfStmt* stmt) {
}

void ASTPrinter::visitNumberExpr(NumberExpr* expr) {
    this->ss << expr->value;
}
void ASTPrinter::visitStringExpr(StringExpr* expr) {
}
void ASTPrinter::visitCharExpr(CharExpr* expr) {
}
void ASTPrinter::visitBoolExpr(BoolExpr* expr) {
}
void ASTPrinter::visitIdentifierExpr(IdentifierExpr* expr) {
    this->ss << expr->value.lexeme;
}
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
    expr->expression->visit(this);
}
void ASTPrinter::visitCallExpr(CallExpr* expr) {
}
