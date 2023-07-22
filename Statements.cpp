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
    for (auto s: _statements){
        s->evaluate(symTab);
    }
}



// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    int rhs = rhsExpression()->evaluate(symTab);
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

PrintStatement::PrintStatement(): _value {""} {}

PrintStatement::PrintStatement(std::string value){_value = value;}

void PrintStatement::print(){
    std::cout << _value << std::endl;
    return;
}

void PrintStatement::evaluate(SymTab &symTab){
    std::cout << symTab.getValueFor(_value) << std::endl;
    return;
}


// For Statement

ForStatement::ForStatement() {_relExpr = nullptr; _firstAssign = nullptr; _secondAssign = nullptr; _statements = nullptr;};

ForStatement::ForStatement(AssignmentStatement *firstAssign, AssignmentStatement *secondAssign, ExprNode *relExpr, Statements *statements){
            _firstAssign = firstAssign;
            _secondAssign = secondAssign;
            _relExpr = relExpr;
            _statements = statements;
};

void ForStatement::print(){
    _firstAssign->print();
    std::cout << std::endl;
    _secondAssign->print();
    std::cout << std::endl;
    _relExpr->print();
    std::cout << std::endl;
    _statements->print();
    std::cout << std::endl;
    return;
}

void ForStatement::evaluate(SymTab &symTab){
    Token tok = _relExpr->token();
    for (_firstAssign->evaluate(symTab); _relExpr->evaluate(symTab); _secondAssign->evaluate(symTab)){
        _statements->evaluate(symTab);
    }
}
