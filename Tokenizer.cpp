//
// Created by Ali Kooshesh on 4/4/23.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

// for my own sake tokenizer constructor should be put on top
Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{} {}

std::string Tokenizer::readName() {

    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);

    return name;
}

int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        intValue = intValue * 10 + c - '0';
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

// Step 2: Create readRelationalOperator to determine what operator it is
// If it is not a proper relational operator, it will exit
// Also handles '=' token to not get a headache dealing with that
std::string Tokenizer::readRelationalOperator(){
    std::string op;
    char c;
    while (inStream.get(c) && c == '!' || c == '>' || c == '<' || c == '='){
        op += c;
    }

    Token temp;
    temp.symbol(op);

    if (!temp.isArithmeticOperator() && !temp.isAssignmentOperator() && !temp.isComparisonOperator()){
        std::cout << "Error: not a relational operator, input was:";
        temp.print();
        exit(1);
    }

    if(inStream.good())
        inStream.putback(c);
    return op;
}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    char c;

    // while( inStream.get(c) && isspace(c) && c != '\n' )  // Skip spaces but not new-line chars.
        ;


    while( inStream.get(c) && isspace(c) );  // Skip spaces including the new-line chars.

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    //    std::cout << "c = " << c << std::endl;
    // sets token object and will determine if it is:
    // eof, eol, isdigit, operator, isalpha
    // if eof: set bool in token
    // if eol: set bool in token
    // if isdigit: put the read char into the stream again to read the whole thing
    // if operator: find which one it is
    // if alpha: must be a name! so call setName from token and readName in Tokenizer
    Token token;
    if( inStream.eof()) {
        token.eof() = true;
    } else if( c == '\n' ) {  // will not ever be the case unless new-line characters are not supressed.
        token.eol() = true;
    } else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber( readInteger() );

    } 
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        token.symbol(c);
    else if( c == ';' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if (c == '{' || c == '}')
        token.symbol(c);
    else if(isalpha(c) && c != '!' && c != '=' && c != '<' && c != '>') {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
    } 
    else if (c == '!' || c == '=' || c == '<' || c == '>'){
        inStream.putback(c);
        token.symbol(readRelationalOperator());
    }
    else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }
    // add token to tokenizer
    _tokens.push_back(token);
    // fail safe to determine eof is met
    return lastToken = token;
}
// ungetToken does shometing somewhere else
void Tokenizer::ungetToken() {
    ungottenToken = true;
}

// just processes the tokens from the vector and prints it out 
void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
