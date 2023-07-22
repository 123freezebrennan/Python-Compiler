#ifndef APYTHONINTERPRETER_TOKENIZER_HPP
#define APYTHONINTERPRETER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include <stack>
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

    int getIndent();

private:
    // used to determine if the last token is eof
    Token lastToken;
    // 
    bool ungottenToken;
    std::ifstream &inStream;
    std::vector<Token> _tokens;

private:
    std::stack<int> indenter;
    bool parsingANewLine;
    
    std::string readName();
    std::string readInteger();
    std::string readRelationalOperator();
    std::string readDashes();
    std::string readString();
};

#endif //APYTHONINTERPRETER_TOKENIZER_HPP
