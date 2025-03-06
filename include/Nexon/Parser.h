#ifndef NEXON_PARSER_H
#define NEXON_PARSER_H

#include "Nexon/AST.h"
#include "Nexon/Lexer.h"
#include <memory>
#include <vector>
#include <iostream>

namespace Nexon {

    // Parser implements a production-ready recursive descent parser.
    class Parser {
    public:
        Parser(const std::string &input);
        int getCurrentToken();
        int getNextToken();
        std::unique_ptr<ExprAST> parseExpression();
        std::unique_ptr<ExprAST> parsePrimary();
        std::unique_ptr<ExprAST> parseIdentifierExpr();
        std::unique_ptr<ExprAST> parseNumberExpr();
        std::unique_ptr<ExprAST> parseParenExpr();
        std::unique_ptr<ExprAST> parseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
        std::unique_ptr<PrototypeAST> parsePrototype();
        std::unique_ptr<FunctionAST> parseDefinition();
        std::unique_ptr<FunctionAST> parseTopLevelExpr();
        int getToken() const { return CurTok; }
    private:
        Lexer Lex;
        int CurTok;
    };

}
#endif // NEXON_PARSER_H
