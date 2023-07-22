//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_TOKEN_HPP
#define APYTHONINTERPRETER_TOKEN_HPP

#include<string>

class Token {

public:
    // contructor
    Token();
    // if value is eof or eol 
    // (end of field or end of line)
    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }
    // return value if it is or is not
    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }
    // if it is open or close parentheses bool return
    bool isOpenParen() const  { return _symbol == "("; }
    bool isCloseParen() const { return _symbol == ")"; }

    bool isOpenBraces() const {return _symbol == "{";}
     bool isCloseBraces() const {return _symbol == "}";}
    // sets _symbol (the operator/semicolon) and the return of what the _symbol is 
    
    void symbol(std::string c) { _symbol = c; }
    void symbol(char c) { _symbol += c; }
    std::string symbol() { return _symbol; }

    // big ol' amount of methods to determine what the char is 
    // (operator, addition, etc.)

    // semicolon is not gonna be used but keeping just in case
    bool isSemiColon() const              { return _symbol == ";"; }
    bool isAssignmentOperator() const     { return _symbol == "="; }
    bool isMultiplicationOperator() const { return _symbol == "*"; }
    bool isAdditionOperator() const       { return _symbol == "+"; }
    bool isSubtractionOperator() const    { return _symbol == "-"; }
    bool isModuloOperator() const         { return _symbol == "%"; }
    bool isDivisionOperator() const       { return _symbol == "/"; }

    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }

    // Step 2 add boolean methods to determine if it is a relational operator
    bool isLessThanOperator() const           {return _symbol == "<";}
    bool isGreaterThanOperator() const        {return _symbol == ">";}
    bool lessThanOrEqualToOperator() const    {return _symbol == "<=";}
    bool greaterThanOrEqualToOperator() const {return _symbol == ">=";}
    bool equalToOperator() const              {return _symbol == "==";}
    bool notEqualToOperator() const           {return _symbol == "!=";}

    bool isComparisonOperator() const {
        return isLessThanOperator() || 
        isGreaterThanOperator() || 
        lessThanOrEqualToOperator() || 
        greaterThanOrEqualToOperator() || 
        equalToOperator() || 
        notEqualToOperator();
    }

    bool isKeyWord() const {return _name == "print" || _name == "for";}
    bool isPrint() const {return _name == "print";}
    bool isFor() const {return _name == "for";}

    // determines if the token is a name of a variable or function etc.
    bool isName() const                   { return _name.length() > 0; }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }

    // if token is a whole number (solid 32 bit int)
    bool &isWholeNumber() { return _is_whole_number; }
    bool isWholeNumber() const { return _is_whole_number; }
    int getWholeNumber() const { return _whole_number; }
    void setWholeNumber(int n) {
        _whole_number = n;
        isWholeNumber() = true;
    }
    // print out defined in the classes .cpp
    void print() const;

private:
    // declaration of the possible token datatypes and setting them as such
    // Step 2: change _symbol from char to a string to handle relational operators
    std::string _name;
    bool _eof, _eol;
    bool _is_whole_number;
    std::string _symbol;
    int _whole_number;
};


#endif //APYTHONINTERPRETER_TOKEN_HPP
