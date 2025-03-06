#ifndef NEXON_LEXER_H
#define NEXON_LEXER_H

#include <string>
#include <cctype>
#include <sstream>
#include <vector>
#include <iostream>

namespace Nexon {

    enum Token {
        tok_eof = -1,
        tok_def = -2,
        tok_extern = -3,
        tok_identifier = -4,
        tok_number = -5,
        tok_keyword = -6,
        tok_operator = -7,
        tok_separator = -8
    };

    // Lexer tokenizes Nexon source code into tokens.
    class Lexer {
    public:
        Lexer(const std::string &input) : Input(input), Position(0) { }
        int getNextToken();
        std::string getIdentifierStr() const { return IdentifierStr; }
        double getNumVal() const { return NumVal; }
    private:
        std::string Input;
        size_t Position;
        std::string IdentifierStr;
        double NumVal;
        char getNextChar();
        char peekChar() const;
    };

}
#endif // NEXON_LEXER_H
