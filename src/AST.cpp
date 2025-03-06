#include "Nexon/AST.h"
#include "Nexon/CodeGen.h"
#include <iostream>
#include "llvm/IR/Verifier.h"

namespace Nexon {
using namespace llvm;

Value* NumberExprAST::codegen() {
    return ConstantFP::get(CodeGen::getGlobalContext(), APFloat(Val));
}

Value* VariableExprAST::codegen() {
    Value* V = CodeGen::getNamedValue(Name);
    if (!V) {
        std::cerr << "Error: Unknown variable " << Name << "\n";
        return nullptr;
    }
    return V;
}

Value* BinaryExprAST::codegen() {
    Value* L = LHS->codegen();
    Value* R = RHS->codegen();
    if (!L || !R)
        return nullptr;
    switch (Op) {
        case '+': return CodeGen::Builder()->CreateFAdd(L, R, "addtmp");
        case '-': return CodeGen::Builder()->CreateFSub(L, R, "subtmp");
        case '*': return CodeGen::Builder()->CreateFMul(L, R, "multmp");
        case '/': return CodeGen::Builder()->CreateFDiv(L, R, "divtmp");
        default:
            std::cerr << "Error: Unknown binary operator " << Op << "\n";
            return nullptr;
    }
}

Value* CallExprAST::codegen() {
    Function* CalleeF = CodeGen::TheModule()->getFunction(Callee);
    if (!CalleeF) {
        std::cerr << "Error: Function " << Callee << " not found.\n";
        return nullptr;
    }
    if (CalleeF->arg_size() != Args.size()) {
        std::cerr << "Error: Incorrect number of arguments for function " << Callee << "\n";
        return nullptr;
    }
    std::vector<Value*> ArgsV;
    for (unsigned i = 0; i < Args.size(); ++i) {
        ArgsV.push_back(Args[i]->codegen());
        if (!ArgsV.back())
            return nullptr;
    }
    return CodeGen::Builder()->CreateCall(CalleeF, ArgsV, "calltmp");
}

Function* PrototypeAST::codegen() {
    std::vector<Type*> Doubles(Args.size(), Type::getDoubleTy(CodeGen::getGlobalContext()));
    FunctionType* FT = FunctionType::get(Type::getDoubleTy(CodeGen::getGlobalContext()), Doubles, false);
    Function* F = Function::Create(FT, Function::ExternalLinkage, getName(), CodeGen::TheModule());
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(Args[Idx++]);
    return F;
}

Function* FunctionAST::codegen() {
    Function* TheFunction = CodeGen::TheModule()->getFunction(Proto->getName());
    if (!TheFunction)
        TheFunction = Proto->codegen();
    if (!TheFunction)
        return nullptr;
    BasicBlock* BB = BasicBlock::Create(CodeGen::getGlobalContext(), "entry", TheFunction);
    CodeGen::Builder()->SetInsertPoint(BB);
    CodeGen::clearNamedValues();
    for (auto &Arg : TheFunction->args())
        CodeGen::setNamedValue(std::string(Arg.getName()), &Arg);
    if (Value* RetVal = Body->codegen()) {
        CodeGen::Builder()->CreateRet(RetVal);
        verifyFunction(*TheFunction);
        return TheFunction;
    }
    TheFunction->eraseFromParent();
    return nullptr;
}

void additionalASTRoutine() {
    for (int i = 0; i < 50; ++i) {
        std::cerr << "AST processing iteration " << i << "\n";
    }
}
}
