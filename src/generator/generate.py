import pathlib
from typing import Dict, List, Tuple

STMT_HEADER = (
    str(pathlib.Path(__file__).parent.parent.resolve()) + "/include/statements.h"
)
STMT_IMPL = "../statements.cpp"
EXPR_HEADER = (
    str(pathlib.Path(__file__).parent.parent.resolve()) + "/include/expressions.h"
)
EXPR_IMPL = "../statements.cpp"


def writeHeader(kind: Tuple[str, str], items: Dict[str, str], extra: str, sb: List):
    short, long = kind
    sb.append(f"#ifndef {long.upper()}_H")
    sb.append(f"#define {long.upper()}_H")
    sb.append("#include <vector>")
    sb.append("#include <string>")
    sb.append('#include "token.h"')
    sb.append("")
    sb.append(extra)
    sb.append("")
    for name in items.keys():
        sb.append(f"class {name}{short};")
    sb.append("")
    sb.append(f"class Visit{short} " + "{")
    sb.append("public:")
    for name in items.keys():
        sb.append(f"virtual void visit{name}{short}({name}{short}* {short.lower()});")
    sb.append(f"virtual ~Visit{short}() = 0;")
    sb.append("};")
    sb.append("")
    sb.append(f"class {short} " + "{")
    sb.append("public:")
    sb.append(f"virtual void visit(Visit{short} visitor) = 0;")
    sb.append(f"virtual ~{short}() = 0;")
    sb.append("};")
    sb.append("")
    for name, fields in items.items():
        sb.append(f"class {name}{short} : {short} " + "{")
        sb.append("public:")
        for field in fields.split(","):
            field = field.strip()
            sb.append(f"{field};")
        sb.append(f"void visit(Visit{short}* visitor) " + "{")
        sb.append(f"visitor->visit{name}{short}(this);")
        sb.append("}")
        sb.append("};")
    sb.append("")
    sb.append("#endif")


def main():
    stmts = {
        "Block": "std::vector<std::unique_ptr<Stmt>> body",
        "Var": "Token name, Type type, std::unique_ptr<Expr> initializer",
        "Fn": "Token name, std::vector<Param> params, std::unique_ptr<Stmt> body, Type returnType",
        "Expression": "std::unique_ptr<Expr> expression",
        "Return": "std::unique_ptr<Expr> expression",
        "If": "std::unique_ptr<Expr> ifCondition, std::unique_ptr<Stmt> ifBlock, std::vector<Expr> elifConditions, std::vector<Stmt> elifBlocks, std::unique_ptr<Stmt> elseBlock",
    }
    extra = """
    #include "expressions.h"
    struct Type {
        Token token;
        bool isPointer;
    };
    struct Param {
        Token name;
        Type type;
    };
    """
    stmtStringBuilder = []
    writeHeader(("Stmt", "Statements"), stmts, extra, stmtStringBuilder)
    with open(STMT_HEADER, "w") as f:
        f.write("\n".join(stmtStringBuilder))

    exprs = {
        "Number": "std::string value",
        "String": "std::string value",
        "Char": "std::string value",
        "Bool": "std::string value",
        "Identifier": "Token value",
        "Binary": "std::unique_ptr<Expr> left, Token oper, std::unique_ptr<Expr> right",
        "Unary": "Token oper, std::unique_ptr<Expr> right",
        "Grouping": "std::unique_ptr<Expr> expression",
        "Call": "std::unique_ptr<Expr> callee, std::vector<Expr> args",
    }
    exprStringBuilder = []
    writeHeader(("Expr", "Expressions"), exprs, "", exprStringBuilder)
    with open(EXPR_HEADER, "w") as f:
        f.write("\n".join(exprStringBuilder))


if __name__ == "__main__":
    main()
