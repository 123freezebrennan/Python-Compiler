#include <iostream>
#include "Token.hpp"

// Step 2: Change the _symbol in the constructor to be a string, and add to the print statement with all
// relational operators from the .hpp 

// sets all possible datatypes to base values
Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{"\0"}, _is_dash{false}, _isWholeNumber{false}, _string{""}, _is_string{false}, _is_pos{false} {}

// prints out what the value is based on the big ol bool pool
void Token::print() const {
         if ( eol() )               std::cout << "EOL" ;
    else if ( eof() )          std::cout << "EOF" ;
    else if ( isOpenParen() )  std::cout << "(" ;
    else if ( isCloseParen() ) std::cout << ")" ;
    else if ( isOpenBraces()) std::cout << "{";
    else if ( isCloseBraces()) std::cout << "}";
    else if ( isAssignmentOperator() )  std::cout << " = " ;
    else if ( isSemiColon() )  std::cout << ";" ;
    else if ( isComma() )  std::cout << "," ;
    else if ( isMultiplicationOperator() )  std::cout << " * " ;
    else if ( isAdditionOperator() )  std::cout << " + " ;
    else if ( isSubtractionOperator() )  std::cout << " - " ;
    else if ( isModuloOperator() )  std::cout << " % " ;
    else if ( isDivisionOperator() )  std::cout << " / " ;
    else if ( isIntDivision() )  std::cout << " // " ;
    else if ( isLessThanOperator()) std::cout << " < ";
    else if ( isGreaterThanOperator()) std::cout << " > ";
    else if ( lessThanOrEqualToOperator()) std::cout << " <= ";
    else if ( greaterThanOrEqualToOperator()) std::cout << " >= ";
    else if ( isNotEqualOther()) std::cout << " <> ";
    else if ( equalToOperator()) std::cout << " == ";
    else if ( notEqualToOperator()) std::cout << " != ";
    else if ( isName() )  std::cout << getName();
    else if ( isWholeNumber() ) std::cout << getWholeNumber();
    else if ( isDashSymbol() ) std::cout << getDashSymbol();
    else if (isDoubleNumber()) std::cout << getDoubleNumber();
    else if (getString().length() > 0) std::cout << getString();
    else if (isComment()) std::cout << getComment();
    else if (isPrint()) std::cout << getName();
    else if (isFor()) std::cout << getName();
    else if (isRange()) std::cout << getName();
    else if (isIn()) std::cout << getName();
    else if (isOr()) std::cout << getName();
    else if (isAnd()) std::cout << getName();
    else if (isNot()) std::cout << getName();
    else if (isColon()) std::cout << ":";
    else if (isIf()) std::cout << getName();
    else if (isElif()) std::cout << getName();
    else if (isElse()) std::cout << getName();
    else std::cout << "Uninitialized token.\n";
}
