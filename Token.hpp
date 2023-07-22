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
    
    bool isQuote() const {return _symbol == "\"";}  
      
    bool &isString() {return _is_string;}
    std::string getString() const {return _string;}
    void setString(std::string input) {_string = input; _is_string = true;}

    bool isDashSymbol() const             { return _is_dash; }
    void setDashSymbol(std::string dashes) {
        _string = dashes;
        if (_string.length() % 2 == 0)
            _is_pos = true;
        _is_dash = true;
    }
    std::string getDashSymbol() const {return _string;}
    bool isPositive() const {return _is_pos;}

    void symbol(std::string c) { _symbol = c; }
    void symbol(char c) { _symbol += c; }
    std::string symbol() { return _symbol; }


    void setComment(std::string c){_comment = c; _is_comment = true;}
    bool isComment() const {return _is_comment;}
    std::string getComment() const {return _comment;}
    // big ol' amount of methods to determine what the char is 
    // (operator, addition, etc.)

    // semicolon is not gonna be used but keeping just in case
    bool isComma() const {return _symbol == ",";}
    bool getComma() const {return " ";}
    bool isSemiColon() const              { return _symbol == ";"; }
    bool isColon() const                  { return _symbol == ":";}
    bool isAssignmentOperator() const     { return _symbol == "="; }
    bool isMultiplicationOperator() const { return _symbol == "*"; }
    bool isAdditionOperator() const       { return _symbol == "+"; }
    bool isSubtractionOperator() const    { return _symbol == "-"; }
    bool isModuloOperator() const         { return _symbol == "%"; }
    bool isDivisionOperator() const       { return _symbol == "/"; }
    bool isIntDivision() const            { return _symbol == "//";}
    bool isNotEqualOther() const          { return _symbol == "<>";}

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

    bool isRelationalOperator() const {
        return isLessThanOperator() || 
        isGreaterThanOperator() || 
        lessThanOrEqualToOperator() || 
        greaterThanOrEqualToOperator() || 
        equalToOperator() || 
        notEqualToOperator();
    }

    bool isKeyWord() const {
    return 
        _name == "print" || 
        _name == "for" || 
        _name == "range" ||
        _name == "in" || 
        _name == "or" || 
        _name == "and" ||
        _name == "not"  || 
        _name == "if" || 
        _name == "elif" || 
        _name == "else";
    }
    bool isPrint() const {return _name == "print";}
    bool isFor() const {return _name == "for";}
    bool isIn() const {return _name == "in";}
    bool isRange() const {return _name == "range";}
    bool isOr() const {return _name == "or";}
    bool isAnd() const {return _name == "and";}
    bool isNot() const {return _name == "not";}
    bool isIf() const {return _name == "if";}
    bool isElif() const {return _name == "elif";}
    bool isElse() const {return _name == "else";}
    // determines if the token is a name of a variable or function etc.
    bool isName() const                   { return _name.length() > 0 && !isKeyWord(); }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }

    // if token is a whole number (solid 32 bit int)
    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    bool &isDoubleNumber() {return _isDoubleNumber;}
    bool isDoubleNumber() const {return _isDoubleNumber;}
    double getDoubleNumber() const {return _doubleNumber;}
    void setDoubleNumber(double n){
        _doubleNumber = n;
        isDoubleNumber() = true;
    }
    // print out defined in the classes .cpp
    void print() const;

    operator bool() const {
        if (isDoubleNumber())
            return _doubleNumber != 0;
        
        else if (isWholeNumber())
            return _wholeNumber != 0;
        
        else if (getString().length() > 0){
            std::cout << "Error: Token::bool - strings are not allowed\n";
            exit(1);
        }
    }

private:
    // declaration of the possible token datatypes and setting them as such
    // Step 2: change _symbol from char to a string to handle relational operators
    std::string _name;

    bool _eof, _eol;
    bool _isWholeNumber;
    bool _isDoubleNumber;
    bool _is_string;
    bool _is_comment;

    bool _is_pos;
    bool _is_dash;
    std::string _symbol;
    int _wholeNumber;
    double _doubleNumber;
    std::string _string;

    std::string _comment;


};


#endif //APYTHONINTERPRETER_TOKEN_HPP
