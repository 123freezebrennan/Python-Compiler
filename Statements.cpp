#include "Statements.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}

void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

int Statements::getAmountOfStatements(){
    return _statements.size();
}

// AssignmentStatement
void printValue(TypeDescriptor *desc) {
    // If desc is of datatype NumberDescriptor, this function
    // print the value that is stored in it. Otherwise,
    // it just returns.
    // Use dynamic_cast to down-cast, from TypeDescriptor to
    // NumberDescriptr.
    if (desc->type() == TypeDescriptor::STRING){
        StringDescriptor *nDesc = dynamic_cast<StringDescriptor *>(desc);
        std::cout << nDesc->value << std::endl;
        return;
    }
    NumberDescriptor *nDesc = dynamic_cast<NumberDescriptor *>(desc);
    // dynamic_cast will return a nullptr if
    // desc is not of datatype NumberDescritpr.
    if( nDesc == nullptr )
        return;
    // desc must have been of type NumberDescriptr
    if( nDesc->type() == TypeDescriptor::INTEGER )
        std::cout << nDesc->value.intValue << std::endl;
    else if( nDesc->type() == TypeDescriptor::DOUBLE )
        std::cout << nDesc->value.doubleValue << std::endl;
    else
        std::cout << "Misconfigured union type." << std::endl;
}

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    TypeDescriptor *rhs = rhsExpression()->evaluate(symTab);

    symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}

// Print Statement

PrintStatement::PrintStatement(): _values {nullptr} {}

void PrintStatement::print(){
    for (int i = 0; i < _values.size(); i += 1){
        _values.at(i)->print();
    }
    return;
}

void PrintStatement::evaluate(SymTab &symTab){

    for (int i = 0; i < _values.size(); i += 1){
        if (NumberDescriptor *value_int = dynamic_cast<NumberDescriptor *>(_values.at(i)->evaluate(symTab))){
            if( value_int->type() == TypeDescriptor::INTEGER )
                std::cout << value_int->value.intValue;
            else if( value_int->type() == TypeDescriptor::DOUBLE )
                std::cout << value_int->value.doubleValue;
        }
        else {
            StringDescriptor* value = dynamic_cast<StringDescriptor*>(_values.at(i)->evaluate(symTab));
            std::cout << value->value;
        }
    }
    std::cout << std::endl;

    return;
}


IfStatement::IfStatement() {
    _test_if = nullptr;
    _test_if = nullptr;
    _suite_if = nullptr;
    _is_elifs = false;
    _suite_else = nullptr;
    _is_else = false;
    
}

IfStatement::IfStatement(
ExprNode* test_if, 
Statements* suite_if, 
std::vector<ExprNode*> test_elifs, 
std::vector <Statements*> suite_elifs, 
bool is_elifs,
Statements* suite_else, 
bool is_else) {

    _test_if = test_if;
    _suite_if = suite_if;
    _test_elifs = test_elifs;
    _suite_elifs = suite_elifs;
    _is_elifs = is_elifs;
    _suite_else = suite_else;
    _is_else = is_else;

}

void IfStatement::print() {
    _test_if->print();
    _suite_if->print();

    if (_is_elifs) {
        for (int i = 0; i < _suite_elifs.size(); i += 1){
            _test_elifs.at(i)->print();
            _suite_elifs.at(i)->print();
        }
    }

    if (_is_else) {
        _suite_else->print();
    }

    return;
}

void IfStatement::evaluate(SymTab &symTab) {
    bool did_elif = false;
    if(dynamic_cast<NumberDescriptor *>(_test_if->evaluate(symTab))->value.intValue){
        _suite_if->evaluate(symTab);
        return;
    }
    if (_is_elifs){

        for (int i = 0; i < _test_elifs.size(); i++){
            if(dynamic_cast<NumberDescriptor *>(_test_elifs.at(i)->evaluate(symTab))->value.intValue){
                _suite_elifs.at(i)->evaluate(symTab);
                did_elif = true;
                return;
            }
            else{
                continue;
            }
        }
    }
    
    if (_is_else && !did_elif){
        _suite_else->evaluate(symTab);
        return;

    }
    
    return;
}

// For Statement

ForStatement::ForStatement() {_range = nullptr; _value = nullptr; _statements = nullptr;};

ForStatement::ForStatement(std::string value, Range *range, Statements *statements) {
    _value = value;
    _range = range;
    _statements = statements;
};

void ForStatement::print(){
    std::cout << _value << std::endl;
    std::cout << std::endl;
    _statements->print();
    std::cout << std::endl;
    return;
}

void ForStatement::evaluate(SymTab &symTab){

    int range_temp = _range->getRange();
    int init_temp = _range->getInit();

    NumberDescriptor* start = new NumberDescriptor(TypeDescriptor::INTEGER);
    start->value.intValue = _range->getInit();
    symTab.setValueFor(_value, start);

    while (_range->condition()){
        _statements->evaluate(symTab);
        
        NumberDescriptor* iterate = new NumberDescriptor(TypeDescriptor::INTEGER);
        iterate->value.intValue = _range->next();
        symTab.setValueFor(_value, iterate);
    }

    _range = new Range(init_temp, range_temp, _range->getStep());
    return;
}



Range::Range(int rangeValue){
    _rangeValue = rangeValue;
    _initValue = 0;
    _stepValue = 1;
    is_reverse = false;
}

Range::Range(int initValue, int rangeValue){
    _initValue = initValue;
    _rangeValue = rangeValue;
    _stepValue = 1;
    is_reverse = false;
}

Range::Range(int initValue, int rangeValue, int stepValue){
    _initValue = initValue;
    _rangeValue = rangeValue;
    _stepValue = stepValue;
    if (_rangeValue < _initValue)
        is_reverse = true;
    else
        is_reverse = false;
}


