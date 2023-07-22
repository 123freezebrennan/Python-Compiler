//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_TOKENIZER_HPP
#define APYTHONINTERPRETER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include "Token.hpp"

// sets up a vector of tokens and defines what the tokens are
class Tokenizer {

public:
    // constructor takes in the filestream to define the tokens 
    Tokenizer(std::ifstream &inStream);
    // returns token 
    Token getToken();
    // unsets a token possibly
    void ungetToken();
    // pretty explanatory
    void printProcessedTokens();

private:
    // used to determine if the last token is eof
    Token lastToken;
    // 
    bool ungottenToken;
    std::ifstream &inStream;
    std::vector<Token> _tokens;

private:
    std::string readName();
    int readInteger();
    std::string readRelationalOperator();
};

#endif //APYTHONINTERPRETER_TOKENIZER_HPP
