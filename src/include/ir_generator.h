#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "expressions.h"
#include "statements.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>

class IRGenerator : public VisitStmt, public VisitExpr<llvm::Value*> {
  public:
    IRGenerator();
    ~IRGenerator();
    void generateIR(std::string moduleName, std::ostream& output, const std::vector<std::unique_ptr<Stmt>>& stmts);

  private:
    std::unique_ptr<llvm::LLVMContext> ctx;
    std::unique_ptr<llvm::Module>      module;
    std::unique_ptr<llvm::IRBuilder<>> builder;

    void execute(const std::unique_ptr<Stmt>& stmt);
    void evaluate(const std::unique_ptr<Expr>& expr);

    void visitBlockStmt(BlockStmt* stmt) override;
    void visitVarStmt(VarStmt* stmt) override;
    void visitFnStmt(FnStmt* stmt) override;
    void visitExpressionStmt(ExpressionStmt* stmt) override;
    void visitReturnStmt(ReturnStmt* stmt) override;
    void visitIfStmt(IfStmt* stmt) override;

    llvm::Value* visitNumberExpr(NumberExpr* expr) override;
    llvm::Value* visitStringExpr(StringExpr* expr) override;
    llvm::Value* visitCharExpr(CharExpr* expr) override;
    llvm::Value* visitBoolExpr(BoolExpr* expr) override;
    llvm::Value* visitIdentifierExpr(IdentifierExpr* expr) override;
    llvm::Value* visitBinaryExpr(BinaryExpr* expr) override;
    llvm::Value* visitUnaryExpr(UnaryExpr* expr) override;
    llvm::Value* visitGroupingExpr(GroupingExpr* expr) override;
    llvm::Value* visitCallExpr(CallExpr* expr) override;
};

#endif
