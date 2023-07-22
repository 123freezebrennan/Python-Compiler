//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_STATEMENTS_HPP
#define APYTHONINTERPRETER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "ExprNode.hpp"
#include "SymTab.hpp"
// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.


class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;
};

// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.
class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);
    int getAmountOfStatements();
    void print();
private:
    std::vector<Statement *> _statements;
};


// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

// Prints the evaluated variable name stored as a string 
class PrintStatement : public Statement {
    public:
        PrintStatement();
        PrintStatement(std::vector<ExprNode*> values){_values = values;};
        void print();

        void evaluate(SymTab &symTab);
    private:
        std::vector<ExprNode*> _values;

};

class Range {
public:
      Range(int rangeValue);  // set initValue to zero and stepValue to 1.
      Range(int initValue, int rangeValue);  // set stepVlaue to 1.
      Range(int initValue, int rangeValue, int stepValue);

      bool condition(){
        if (is_reverse)
            return _initValue > _rangeValue;

        else
            return _initValue < _rangeValue;
            
        
        }; // should we iterate?
      int next() { _initValue += _stepValue; return _initValue;};       // the value to be assigned to the loop counter.
      int getRange() {return _rangeValue;};
      int getInit() {return _initValue;};
      int getStep() {return _stepValue;};


private:
      int _initValue, _stepValue, _rangeValue;
      bool is_reverse;
};

// Runs for loop of all needed values within the statement
// if: _statements contains any statements, then those will run in a recursive manner.

class ForStatement : public Statement {
    public:
        ForStatement();
        
        ForStatement(std::string, Range *range, Statements *statements);

        void print();

        void evaluate(SymTab & symTab);

    private:
        std::string _value;
        Range *_range;
        Statements *_statements;  
};

class IfStatement: public Statement {
    public:
        IfStatement();
        IfStatement(
        ExprNode* test_if, 
        Statements* suite_if, 
        std::vector<ExprNode*> test_elifs, 
        std::vector <Statements*> suite_elifs, 
        bool is_elifs, 
        Statements* suite_else, 
        bool is_else);
        
        void print();
        void evaluate(SymTab &symTab);
        
    private:
        ExprNode *_test_if;
        Statements* _suite_if;

        std::vector<ExprNode*> _test_elifs;
        std::vector <Statements*> _suite_elifs;
        bool _is_elifs;

        Statements* _suite_else;
        bool _is_else;
};






#endif //APYTHONINTERPRETER_STATEMENTS_HPP
