#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H
#include <vector>
#include <string>
#include "token.h"



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
virtual void visitNumberExpr(NumberExpr* expr) = 0;
virtual void visitStringExpr(StringExpr* expr) = 0;
virtual void visitCharExpr(CharExpr* expr) = 0;
virtual void visitBoolExpr(BoolExpr* expr) = 0;
virtual void visitIdentifierExpr(IdentifierExpr* expr) = 0;
virtual void visitBinaryExpr(BinaryExpr* expr) = 0;
virtual void visitUnaryExpr(UnaryExpr* expr) = 0;
virtual void visitGroupingExpr(GroupingExpr* expr) = 0;
virtual void visitCallExpr(CallExpr* expr) = 0;
virtual ~VisitExpr() = default;
};

class Expr {
public:
virtual void visit(VisitExpr* visitor) = 0;
virtual ~Expr() = default;
};

class NumberExpr : public Expr {
public:
~NumberExpr(){};
NumberExpr() : 
value(){}
double value;
void visit(VisitExpr* visitor) {
visitor->visitNumberExpr(this);
}
};
class StringExpr : public Expr {
public:
~StringExpr(){};
StringExpr() : 
value(){}
std::string value;
void visit(VisitExpr* visitor) {
visitor->visitStringExpr(this);
}
};
class CharExpr : public Expr {
public:
~CharExpr(){};
CharExpr() : 
value(){}
std::string value;
void visit(VisitExpr* visitor) {
visitor->visitCharExpr(this);
}
};
class BoolExpr : public Expr {
public:
~BoolExpr(){};
BoolExpr() : 
value(){}
bool value;
void visit(VisitExpr* visitor) {
visitor->visitBoolExpr(this);
}
};
class IdentifierExpr : public Expr {
public:
~IdentifierExpr(){};
IdentifierExpr() : 
value(){}
Token value;
void visit(VisitExpr* visitor) {
visitor->visitIdentifierExpr(this);
}
};
class BinaryExpr : public Expr {
public:
~BinaryExpr(){};
BinaryExpr() : 
left(nullptr),oper(),right(nullptr){}
std::unique_ptr<Expr> left;
Token oper;
std::unique_ptr<Expr> right;
void visit(VisitExpr* visitor) {
visitor->visitBinaryExpr(this);
}
};
class UnaryExpr : public Expr {
public:
~UnaryExpr(){};
UnaryExpr() : 
oper(),right(nullptr){}
Token oper;
std::unique_ptr<Expr> right;
void visit(VisitExpr* visitor) {
visitor->visitUnaryExpr(this);
}
};
class GroupingExpr : public Expr {
public:
~GroupingExpr(){};
GroupingExpr() : 
expression(nullptr){}
std::unique_ptr<Expr> expression;
void visit(VisitExpr* visitor) {
visitor->visitGroupingExpr(this);
}
};
class CallExpr : public Expr {
public:
~CallExpr(){};
CallExpr() : 
callee(nullptr),args(){}
std::unique_ptr<Expr> callee;
std::vector<Expr> args;
void visit(VisitExpr* visitor) {
visitor->visitCallExpr(this);
}
};

#endif