#include "Nexon/Lexer.h"
#include <cstdlib>
#include <iostream>

namespace Nexon {

char Lexer::getNextChar() {
    if (Position < Input.size())
        return Input[Position++];
    return EOF;
}

char Lexer::peekChar() const {
    if (Position < Input.size())
        return Input[Position];
    return EOF;
}

int Lexer::getNextToken() {
    while (std::isspace(peekChar()))
        getNextChar();
    char CurChar = peekChar();
    if (std::isalpha(CurChar)) {
        IdentifierStr.clear();
        while (std::isalnum(peekChar()))
            IdentifierStr.push_back(getNextChar());
        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }
    if (std::isdigit(CurChar) || CurChar == '.') {
        std::string NumStr;
        while (std::isdigit(peekChar()) || peekChar() == '.')
            NumStr.push_back(getNextChar());
        NumVal = std::strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }
    if (CurChar == '#') {
        while (getNextChar() != '\n' && peekChar() != EOF) { }
        return getNextToken();
    }
    int ThisChar = getNextChar();
    if (ThisChar == EOF)
        return tok_eof;
    return ThisChar;
}

void debugTokens(const std::string &input) {
    Lexer lex(input);
    int token = lex.getNextToken();
    while (token != tok_eof) {
        std::cout << "Token: " << token << ", ";
        if (token == tok_identifier)
            std::cout << "Identifier: " << lex.getIdentifierStr();
        else if (token == tok_number)
            std::cout << "Number: " << lex.getNumVal();
        std::cout << std::endl;
        token = lex.getNextToken();
    }
}

void extraLexerRoutine() {
    for (int i = 0; i < 50; ++i) {
        std::cout << "Extra Lexer routine line " << i << std::endl;
    }
}

void additionalLexerWork() {
    for (int i = 0; i < 20; ++i) {
        std::cout << "Additional Lexer work " << i << std::endl;
    }
}
}
