#include "Nexon/Parser.h"
#include <map>
#include <iostream>

namespace Nexon {

static std::map<int, int> BinopPrecedence = {
    {'<', 10},
    {'+', 20},
    {'-', 20},
    {'*', 40},
    {'/', 40}
};

Parser::Parser(const std::string &input) : Lex(input) {
    CurTok = Lex.getNextToken();
}

int Parser::getCurrentToken() { return CurTok; }
int Parser::getNextToken() { return CurTok = Lex.getNextToken(); }

std::unique_ptr<ExprAST> Parser::parseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(Lex.getNumVal());
    getNextToken();
    return Result;
}

std::unique_ptr<ExprAST> Parser::parseParenExpr() {
    getNextToken(); // Consume '('
    auto V = parseExpression();
    if (getCurrentToken() != ')') {
        std::cerr << "Error: expected ')'." << std::endl;
        return nullptr;
    }
    getNextToken(); // Consume ')'
    return V;
}

std::unique_ptr<ExprAST> Parser::parseIdentifierExpr() {
    std::string IdName = Lex.getIdentifierStr();
    getNextToken();
    if (getCurrentToken() != '(')
        return std::make_unique<VariableExprAST>(IdName);
    getNextToken(); // Consume '('
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (getCurrentToken() != ')') {
        while (true) {
            if (auto Arg = parseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
            if (getCurrentToken() == ')')
                break;
            if (getCurrentToken() != ',') {
                std::cerr << "Error: expected ',' or ')'." << std::endl;
                return nullptr;
            }
            getNextToken();
        }
    }
    getNextToken(); // Consume ')'
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> Parser::parsePrimary() {
    switch (getCurrentToken()) {
        case tok_identifier:
            return parseIdentifierExpr();
        case tok_number:
            return parseNumberExpr();
        case '(':
            return parseParenExpr();
        default:
            std::cerr << "Error: unknown token when expecting an expression." << std::endl;
            return nullptr;
    }
}

std::unique_ptr<ExprAST> Parser::parseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while (true) {
        int TokPrec = BinopPrecedence[getCurrentToken()];
        if (TokPrec < ExprPrec)
            return LHS;
        int BinOp = getCurrentToken();
        getNextToken();
        auto RHS = parsePrimary();
        if (!RHS)
            return nullptr;
        int NextPrec = BinopPrecedence[getCurrentToken()];
        if (TokPrec < NextPrec) {
            RHS = parseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

std::unique_ptr<ExprAST> Parser::parseExpression() {
    auto LHS = parsePrimary();
    if (!LHS)
        return nullptr;
    return parseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<PrototypeAST> Parser::parsePrototype() {
    if (getCurrentToken() != tok_identifier) {
        std::cerr << "Error: expected function name in prototype." << std::endl;
        return nullptr;
    }
    std::string FnName = Lex.getIdentifierStr();
    getNextToken();
    if (getCurrentToken() != '(') {
        std::cerr << "Error: expected '(' in prototype." << std::endl;
        return nullptr;
    }
    getNextToken(); // Consume '('
    std::vector<std::string> ArgNames;
    while (getCurrentToken() == tok_identifier) {
        ArgNames.push_back(Lex.getIdentifierStr());
        getNextToken();
    }
    if (getCurrentToken() != ')') {
        std::cerr << "Error: expected ')' in prototype." << std::endl;
        return nullptr;
    }
    getNextToken(); // Consume ')'
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAST> Parser::parseDefinition() {
    getNextToken(); // Consume 'def'
    auto Proto = parsePrototype();
    if (!Proto)
        return nullptr;
    if (auto E = parseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

std::unique_ptr<FunctionAST> Parser::parseTopLevelExpr() {
    if (auto E = parseExpression()) {
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

void extraParserRoutine() {
    for (int i = 0; i < 50; ++i) {
        std::cout << "Extra parser iteration " << i << std::endl;
    }
}
}
