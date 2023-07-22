//
// Created by Ali Kooshesh on 4/4/23.
//

#include <iostream>
#include <string>

#include "Tokenizer.hpp"

// for my own sake tokenizer constructor should be put on top
Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{}, parsingANewLine{true} {
    indenter.push(0);
}

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

std::string Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    std::string Value = "";
    char c;
    while( inStream.get(c) && (isdigit(c) || c == '.') ) {
        Value += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return Value;
}

std::string Tokenizer::readString(){
    std::string temp = "";
    char c;
    char what_quote = '\'';
    inStream.get(c);
    if (c == '\"'){
        what_quote = '\"';
    }

    temp += c;
    while(inStream.get(c) && c != what_quote){
        temp += c;
    }
    temp += c;
    if (temp.at(0) != temp.at(temp.length() - 1)){
        std::cout << "Error: string determinates must match on either side\n";
        exit(1);
    }
    return temp.substr(1, temp.length()-2);
}

// Step 2: Create readRelationalOperator to determine what operator it is
// If it is not a proper relational operator, it will exit
// Also handles '=' token to not get a headache dealing with that
std::string Tokenizer::readRelationalOperator(){
    std::string op;
    char c;
    while (inStream.get(c) && c == '!' || c == '>' || c == '<' || c == '=')
        op += c;
    

    Token temp;
    temp.symbol(op);

    if (!temp.isArithmeticOperator() && !temp.isAssignmentOperator() && !temp.isRelationalOperator() && !temp.isNotEqualOther()){
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
    int number_of_spaces = 0;

    while( inStream.get(c) && isspace(c) && c != '\n' ){
        number_of_spaces += 1;
    }
    
    // Skip spaces but not new-line chars.
    // std::cout << "number of spaces: " << number_of_spaces << std::endl;
    if (parsingANewLine == true){
        if(indenter.top() < number_of_spaces)
            indenter.push(number_of_spaces);

        else {
            while (indenter.top() != number_of_spaces && !(indenter.top() == 0)){
                if (number_of_spaces > indenter.top()){
                    std::cout << "Error: indentation is illegal\n";
                    exit(1);
                }
                indenter.pop();
                
            }
        }
        parsingANewLine = false;
    }
    // std::cout << "INDENTER: " << indenter.top() << std::endl;
    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    // sets token object and will determine if it is:
    // eof, eol, isdigit, operator, isalpha
    // if eof: set bool in token
    // if eol: set bool in token
    // if isdigit: put the read char into the stream again to read the whole thing
    // if operator: find which one it is
    // if alpha: must be a name! so call setName from token and readName in Tokenizer
    Token token;
    if( inStream.eof()) {
        while (!(indenter.top() == 0))
            indenter.pop();
        token.eof() = true;
    } 
    else if( c == '\n' ) {  // will not ever be the case unless new-line characters are not supressed.
        parsingANewLine = true;
        token.eol() = true;
    }
    else if(c == '#'){
        std::string comment = "";
        comment += c;
        while( inStream.get(c) && c != '\n' && !inStream.eof()){
            comment += c;
        }
        if(inStream.good())
            inStream.putback(c);
        token.setComment(comment);
    }
    else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        std::string place_holder = readInteger();
        bool is_double = false;
        for(int i = 0; i < place_holder.length(); i += 1){
            if (place_holder.at(i) == '.'){
                if (i + 1 != place_holder.length())
                    is_double = true;
                else{
                    std::cout << "Syntax error\n";
                    exit(1);
                }
            }
        }
        if (is_double)
            token.setDoubleNumber(std::stod(place_holder));
        else
            token.setWholeNumber(std::stoi(place_holder));
    } 
    else if( c == '+' ||
             c == '-' ||
             c == '*' || 
             c == '%' || 
             c == ';' || 
             c == '(' || 
             c == ')' || 
             c == '{' || 
             c == '}' ||
             c == ',' || 
             c == ':'
            ){
                token.symbol(c);
            }
    else if (c == '/'){
        std::string div_temp = "/";
        while(inStream.get(c) && c == '/'){div_temp += c;}\

        if(inStream.good())
            inStream.putback(c);
        token.symbol(div_temp);

    }
    else if (c == '\"' || c == '\''){
        inStream.putback(c);
        token.setString(readString());
    }
    else if(isalpha(c) && c != '!' && c != '=' && c != '<' && c != '>') {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        parsingANewLine = false;
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
    if (!token.eol() && number_of_spaces > indenter.top()) 
        parsingANewLine = true;
    
    
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

int Tokenizer::getIndent(){
    return indenter.top();
}
