#include <iostream>
#include "Token.hpp"

// Step 2: Change the _symbol in the constructor to be a string, and add to the print statement with all
// relational operators from the .hpp 

// sets all possible datatypes to base values
Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{"\0"}, _is_whole_number{false} {}

// prints out what the value is based on the big ol bool pool
void Token::print() const {
         if ( eol() )               std::cout << "EOL\n" ;
    else if ( eof() )          std::cout << "EOF" ;
    else if ( isOpenParen() )  std::cout << "(" ;
    else if ( isCloseParen() ) std::cout << ")" ;
    else if ( isOpenBraces()) std::cout << "{";
    else if ( isCloseBraces()) std::cout << "}";
    else if ( isAssignmentOperator() )  std::cout << " = " ;
    else if ( isSemiColon() )  std::cout << ";" ;
    else if ( isMultiplicationOperator() )  std::cout << " * " ;
    else if ( isAdditionOperator() )  std::cout << " + " ;
    else if ( isSubtractionOperator() )  std::cout << " - " ;
    else if ( isModuloOperator() )  std::cout << " % " ;
    else if ( isDivisionOperator() )  std::cout << " / " ;
    else if ( isLessThanOperator()) std::cout << " < ";
    else if ( isGreaterThanOperator()) std::cout << " > ";
    else if ( lessThanOrEqualToOperator()) std::cout << " <= ";
    else if ( greaterThanOrEqualToOperator()) std::cout << " >= ";
    else if ( equalToOperator()) std::cout << " == ";
    else if ( notEqualToOperator()) std::cout << " != ";
    else if ( isName() )  std::cout << getName();
    else if ( isWholeNumber() ) std::cout << getWholeNumber();
    else std::cout << "Uninitialized token.\n";
}
