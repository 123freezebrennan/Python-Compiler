
#include<iostream>
#include "ExprNode.hpp"

// ExprNode
// Originall ArithExprNode

// Constructor for ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

// Returns root node, a Token object
Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

// returns the left branch of the ExprNode
ExprNode *&InfixExprNode::left() { return _left; }

// returns the right branch of the ExprNode
ExprNode *&InfixExprNode::right() { return _right; }

// Evaluates the tree structure stored in the InfixExprNode by going down each branch of the root node first
// And recursively bringing each branch up to a single value, in which the final value will be determined to store
// In the symTab for the ID value stored with the assignStatement

int InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    int lValue = left()->evaluate(symTab);
    int rValue = right()->evaluate(symTab);

    if(debug)
        std::cout << "InfixExprNode::evaluate: " << lValue << " " <<
            token().symbol() << " " << rValue << std::endl;

    // Evaluates the Arithmetic/Relational operators that the Token (root) can be
    if( token().isAdditionOperator() )
        return lValue + rValue;
    else if(token().isSubtractionOperator())
        return lValue - rValue;
    else if(token().isMultiplicationOperator())
        return lValue * rValue;
    else if(token().isDivisionOperator())
        return lValue / rValue; // division by zero?
    else if( token().isModuloOperator() )
        return lValue % rValue;
    else if (token().isLessThanOperator()){

        return lValue < rValue;
    }
    else if (token().isGreaterThanOperator()){

        return lValue > rValue;
    }
    else if (token().lessThanOrEqualToOperator()){

        return lValue <= rValue;
    }
    else if (token().greaterThanOrEqualToOperator()){

        return lValue >= rValue;
    }
    else if (token().equalToOperator()){

        return lValue == rValue;
    }
    else if (token().notEqualToOperator()){

        return lValue != rValue;
    }
    // Base case to exit out in case the Token input did not exist
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

// WHoleNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

int WholeNumber::evaluate(SymTab &symTab) {
    if(debug)
        std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return token().getWholeNumber();
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

int Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    if(debug)
        std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return symTab.getValueFor(token().getName());
}

