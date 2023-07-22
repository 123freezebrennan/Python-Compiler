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

TypeDescriptor *InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    TypeDescriptor *lValue = left()->evaluate(symTab);
    TypeDescriptor *rValue = right()->evaluate(symTab);

    if(debug)
        std::cout << "InfixExprNode::evaluate: " << lValue->type() << " " << token().symbol() << " " << rValue->type() << std::endl;

    // Evaluates the Arithmetic/Relational operators that the Token (root) can be
    if( token().isAdditionOperator() ){
        if (StringDescriptor* strDesc = dynamic_cast<StringDescriptor*>(lValue)){
            if (StringDescriptor* strDesc = dynamic_cast<StringDescriptor*>(rValue))
            {
                StringDescriptor* str = new StringDescriptor(TypeDescriptor::STRING);

                StringDescriptor *left_DescrPtr = dynamic_cast<StringDescriptor*>(lValue);
                StringDescriptor *right_DescrPtr = dynamic_cast<StringDescriptor*>(rValue);

                str->value = left_DescrPtr->value + right_DescrPtr->value;
                return str;
            }
        }
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            if (left_DescrPtr->type() == NumberDescriptor::DOUBLE || right_DescrPtr->type() == NumberDescriptor::DOUBLE){
                NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
                float lValue_temp;
                float rValue_temp;
                if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                    lValue_temp = left_DescrPtr->value.intValue;
                else
                    lValue_temp = left_DescrPtr->value.doubleValue;

                if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                    rValue_temp = right_DescrPtr->value.intValue;
                else
                    rValue_temp = right_DescrPtr->value.doubleValue;

                desc->value.doubleValue = lValue_temp + rValue_temp;

                return desc;
            }
            int lValue_temp = left_DescrPtr->value.intValue;
            int rValue_temp = right_DescrPtr->value.intValue;
            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            desc->value.intValue = lValue_temp + rValue_temp;
            return desc;
        }
        return nullptr;

    }
    else if(token().isSubtractionOperator()){
        if (lValue == nullptr){
            NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);
            if (right_DescrPtr){
                int rValue_temp;
                if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                    rValue_temp = right_DescrPtr->value.intValue;
                else
                    rValue_temp = right_DescrPtr->value.doubleValue;
                NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
                desc->value.intValue = rValue_temp * -1;
                return desc;
            }
        
        }

        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            if (left_DescrPtr->type() == NumberDescriptor::DOUBLE || right_DescrPtr->type() == NumberDescriptor::DOUBLE){
                NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
                float lValue_temp;
                float rValue_temp;
                if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                    lValue_temp = left_DescrPtr->value.intValue;
                else
                    lValue_temp = left_DescrPtr->value.doubleValue;

                if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                    rValue_temp = right_DescrPtr->value.intValue;
                else
                    rValue_temp = right_DescrPtr->value.doubleValue;

                desc->value.doubleValue = lValue_temp - rValue_temp;
                
                return desc;
            }
            int lValue_temp = left_DescrPtr->value.intValue;
            int rValue_temp = right_DescrPtr->value.intValue;
            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp - rValue_temp;
            return desc;
        }
        return nullptr;

    }
    else if(token().isMultiplicationOperator()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            if (left_DescrPtr->type() == NumberDescriptor::DOUBLE || right_DescrPtr->type() == NumberDescriptor::DOUBLE){
                NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
                float lValue_temp;
                float rValue_temp;
                if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                    lValue_temp = left_DescrPtr->value.intValue;
                else
                    lValue_temp = left_DescrPtr->value.doubleValue;

                if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                    rValue_temp = right_DescrPtr->value.intValue;
                else
                    rValue_temp = right_DescrPtr->value.doubleValue;

                desc->value.doubleValue = lValue_temp * rValue_temp;
                
                return desc;
            }
            int lValue_temp = left_DescrPtr->value.intValue;
            int rValue_temp = right_DescrPtr->value.intValue;
            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp * rValue_temp;
            return desc;
        }
        return nullptr;

    }
    else if(token().isDivisionOperator()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            if (left_DescrPtr->type() == NumberDescriptor::DOUBLE || right_DescrPtr->type() == NumberDescriptor::DOUBLE){
                NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
                float lValue_temp;
                float rValue_temp;
                if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                    lValue_temp = left_DescrPtr->value.intValue;
                else
                    lValue_temp = left_DescrPtr->value.doubleValue;

                if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                    rValue_temp = right_DescrPtr->value.intValue;
                else
                    rValue_temp = right_DescrPtr->value.doubleValue;

                desc->value.doubleValue = lValue_temp / rValue_temp;
                
                return desc;
            }
            int lValue_temp = left_DescrPtr->value.intValue;
            int rValue_temp = right_DescrPtr->value.intValue;
            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp / rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if( token().isModuloOperator() ){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            if (left_DescrPtr->type() == NumberDescriptor::DOUBLE || right_DescrPtr->type() == NumberDescriptor::DOUBLE){
                NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
                int lValue_temp;
                int rValue_temp;

                if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                    lValue_temp = left_DescrPtr->value.intValue;
                else
                    lValue_temp = left_DescrPtr->value.doubleValue;

                if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                    rValue_temp = right_DescrPtr->value.intValue;
                else
                    rValue_temp = right_DescrPtr->value.doubleValue;

                desc->value.doubleValue = lValue_temp % rValue_temp;
                
                return desc;
            }
            int lValue_temp = left_DescrPtr->value.intValue;
            int rValue_temp = right_DescrPtr->value.intValue;
            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp % rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if(token().isIntDivision()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            if (left_DescrPtr->type() == NumberDescriptor::DOUBLE || right_DescrPtr->type() == NumberDescriptor::DOUBLE){
                NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
                int lValue_temp;
                int rValue_temp;

                if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                    lValue_temp = left_DescrPtr->value.intValue;
                else
                    lValue_temp = left_DescrPtr->value.doubleValue;

                if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                    rValue_temp = right_DescrPtr->value.intValue;
                else
                    rValue_temp = right_DescrPtr->value.doubleValue;

                desc->value.intValue = lValue_temp / rValue_temp;
                
                return desc;
            }
            int lValue_temp = left_DescrPtr->value.intValue;
            int rValue_temp = right_DescrPtr->value.intValue;
            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp / rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if (token().isLessThanOperator()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            int lValue_temp;
            int rValue_temp;
            if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                lValue_temp = left_DescrPtr->value.intValue;
            else
                lValue_temp = left_DescrPtr->value.doubleValue;

            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp < rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if (token().isGreaterThanOperator()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            int lValue_temp;
            int rValue_temp;
            if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                lValue_temp = left_DescrPtr->value.intValue;
            else
                lValue_temp = left_DescrPtr->value.doubleValue;

            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp > rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if (token().lessThanOrEqualToOperator()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            int lValue_temp;
            int rValue_temp;
            if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                lValue_temp = left_DescrPtr->value.intValue;
            else
                lValue_temp = left_DescrPtr->value.doubleValue;

            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp <= rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if (token().greaterThanOrEqualToOperator()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            int lValue_temp;
            int rValue_temp;
            if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                lValue_temp = left_DescrPtr->value.intValue;
            else
                lValue_temp = left_DescrPtr->value.doubleValue;

            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp >= rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if (token().equalToOperator()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            int lValue_temp;
            int rValue_temp;
            if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                lValue_temp = left_DescrPtr->value.intValue;
            else
                lValue_temp = left_DescrPtr->value.doubleValue;

            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp == rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if (token().notEqualToOperator() || token().isNotEqualOther()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            int lValue_temp;
            int rValue_temp;
            if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                lValue_temp = left_DescrPtr->value.intValue;
            else
                lValue_temp = left_DescrPtr->value.doubleValue;

            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = lValue_temp != rValue_temp;
            return desc;
        }
        return nullptr;
    }
    else if (token().isOr()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            int lValue_temp;
            int rValue_temp;
            if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                lValue_temp = left_DescrPtr->value.intValue;
            else
                lValue_temp = left_DescrPtr->value.doubleValue;

            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition
            desc->value.intValue = int(lValue_temp || rValue_temp);
            return desc;
        }
        return nullptr;
    }
    else if (token().isAnd()){
        NumberDescriptor *left_DescrPtr = dynamic_cast<NumberDescriptor*>(lValue);
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);

        if (left_DescrPtr && right_DescrPtr){
            // need to change to handle num types
            int lValue_temp;
            int rValue_temp;
            if (left_DescrPtr->type() == NumberDescriptor::INTEGER)
                lValue_temp = left_DescrPtr->value.intValue;
            else
                lValue_temp = left_DescrPtr->value.doubleValue;

            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            
            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            // add addition

            desc->value.intValue = int(lValue_temp && rValue_temp);
            return desc;
        }
        return nullptr;
    }
    else if (token().isNot()) {
        NumberDescriptor *right_DescrPtr = dynamic_cast<NumberDescriptor*>(rValue);
        if (right_DescrPtr){
            int rValue_temp;
            if (right_DescrPtr->type() == NumberDescriptor::INTEGER)
                rValue_temp = right_DescrPtr->value.intValue;
            else
                rValue_temp = right_DescrPtr->value.doubleValue;

            NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);

            desc->value.intValue = int(!rValue_temp);
            return desc;
        }

        
        
        return nullptr;
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

// WholeNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

TypeDescriptor* WholeNumber::evaluate(SymTab &symTab) {
    if(debug)
        std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::INTEGER);
    desc->value.intValue = token().getWholeNumber();
    return desc;
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

TypeDescriptor *Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    if(debug)
        std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return symTab.getValueFor(token().getName());
}


String::String(Token token) : ExprNode{token} {}
void String::print(){
    token().print();
}

TypeDescriptor *String::evaluate(SymTab &symTab){
    if(debug)
        std::cout << "String::evaluate: returning " << token().getString() << std::endl;
    StringDescriptor* desc = new StringDescriptor(TypeDescriptor::STRING);
    if (token().isComma()) {
        desc->value = " ";
        return desc;
    }
    desc->value = token().getString();
    return desc;
}

DoubleNumber::DoubleNumber(Token token) : ExprNode(token){}

void DoubleNumber::print(){
    token().print();
}

TypeDescriptor *DoubleNumber::evaluate(SymTab &symTab){
    if(debug)
        std::cout << "DoubleNumber::evaluate: returning " << token().getDoubleNumber() << std::endl;
    NumberDescriptor* desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
    desc->value.doubleValue = token().getDoubleNumber();
    return desc;
}

Placeholder::Placeholder(Token token): ExprNode(token) {}

void Placeholder::print(){
    return;
}

TypeDescriptor* Placeholder::evaluate(SymTab &symTab){
    return nullptr;
}
