#ifndef NEXON_AST_H
#define NEXON_AST_H

#include <string>
#include <vector>
#include <memory>
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"

namespace Nexon {
    using namespace llvm;

    // Base class for all AST nodes.
    class ExprAST {
    public:
        virtual ~ExprAST() = default;
        virtual Value* codegen() = 0;
    };

    // Number literal node.
    class NumberExprAST : public ExprAST {
        double Val;
    public:
        NumberExprAST(double Val) : Val(Val) { }
        Value* codegen() override;
    };

    // Variable reference node.
    class VariableExprAST : public ExprAST {
        std::string Name;
    public:
        VariableExprAST(const std::string &Name) : Name(Name) { }
        Value* codegen() override;
    };

    // Binary operator node.
    class BinaryExprAST : public ExprAST {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;
    public:
        BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
            : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) { }
        Value* codegen() override;
    };

    // Function call node.
    class CallExprAST : public ExprAST {
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;
    public:
        CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) { }
        Value* codegen() override;
    };

    // Function prototype node.
    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Args;
    public:
        PrototypeAST(const std::string &Name, std::vector<std::string> Args)
            : Name(Name), Args(std::move(Args)) { }
        const std::string &getName() const { return Name; }
        Function* codegen();
    };

    // Function definition node.
    class FunctionAST {
        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;
    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) { }
        Function* codegen();
    };
}
#endif // NEXON_AST_H
