#include "include/ir_generator.h"
#include "include/token.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>

using std::fstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

IRGenerator::IRGenerator() {
}
IRGenerator::~IRGenerator() {
}

void IRGenerator::generateIR(string moduleName, std::ostream& output, const vector<unique_ptr<Stmt>>& stmts) {
    this->ctx     = std::make_unique<llvm::LLVMContext>();
    this->module  = std::make_unique<llvm::Module>(moduleName, *this->ctx);
    this->builder = std::make_unique<llvm::IRBuilder<>>(*this->ctx);

    for (const unique_ptr<Stmt>& stmt : stmts) {
        this->execute(stmt);
    }

    string                   moduleString;
    llvm::raw_string_ostream rso(moduleString);
    this->module->print(rso, nullptr);
    rso.flush();

    output << moduleString << std::endl;
}

void IRGenerator::visitBlockStmt(BlockStmt* stmt) {
}
void IRGenerator::visitVarStmt(VarStmt* stmt) {
}
void IRGenerator::visitFnStmt(FnStmt* stmt) {
}
void IRGenerator::visitExpressionStmt(ExpressionStmt* stmt) {
}
void IRGenerator::visitReturnStmt(ReturnStmt* stmt) {
}
void IRGenerator::visitIfStmt(IfStmt* stmt) {
}

llvm::Value* IRGenerator::visitNumberExpr(NumberExpr* expr) {
}
llvm::Value* IRGenerator::visitStringExpr(StringExpr* expr) {
}
llvm::Value* IRGenerator::visitCharExpr(CharExpr* expr) {
}
llvm::Value* IRGenerator::visitBoolExpr(BoolExpr* expr) {
}
llvm::Value* IRGenerator::visitIdentifierExpr(IdentifierExpr* expr) {
}
llvm::Value* IRGenerator::visitBinaryExpr(BinaryExpr* expr) {
    auto lhsVal = expr->left->visit(this);
    auto rhsVal = expr->right->visit(this);
    switch (expr->oper.type) {
    case PLUS:
        return this->builder->CreateFAdd(lhsVal, rhsVal);
    default:
        stringstream ss;
        ss << "unhandled binary operator: " << expr->oper.lexeme;
        throw std::runtime_error(ss.str());
    }
}
llvm::Value* IRGenerator::visitUnaryExpr(UnaryExpr* expr) {
}
llvm::Value* IRGenerator::visitGroupingExpr(GroupingExpr* expr) {
}
llvm::Value* IRGenerator::visitCallExpr(CallExpr* expr) {
}

void IRGenerator::execute(const unique_ptr<Stmt>& stmt) {
    stmt->visit(this);
}
void IRGenerator::evaluate(const unique_ptr<Expr>& expr) {
    expr->visit(this);
}
