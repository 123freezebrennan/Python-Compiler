#ifndef APYTHONINTERPRETER_PARSER_HPP
#define APYTHONINTERPRETER_PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "ExprNode.hpp"
#include "Statements.hpp"

#include<vector>
#include<iostream>
#include<map>

// Parser will 'parse' through the tokenizer vector and determine if the inputted file matches the syntax
// and will then organize the tokenizer into correct Statements (assignStatement, printStatement, forStatement)
// To be used into the SymTab

class Parser {
    public:
        // Constructor that takes a Tokenizer object and initializes the private tokenizer variable with it.
        Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}
        // Parses and returns Statements.
        Statements *stmt();

        // Parses and returns a simple statement.
        Statement *simple_stmt();

        // Parses and returns a print statement.
        PrintStatement *print_stmt();

        // Parses and returns an assignment statement.
        AssignmentStatement *assign_stmt();

        // Parses and returns a compound statement.
        Statement *compound_stmt();

        // Parses and returns an if statement.
        IfStatement *ifStatement();

        // Parses and returns a for statement.
        ForStatement *forStatement();

        // Parses and returns a suite of statements.
        Statements *suite();

        // Parses and returns a list of expressions.
        std::vector<ExprNode*> testlist();

        // Parses and returns an expression node.
        ExprNode *test();

        // Parses and returns an OR expression node.
        ExprNode *or_test();

        // Parses and returns an AND expression node.
        ExprNode *and_test();

        // Parses and returns a NOT expression node.
        ExprNode *not_test();

        // Parses and returns a comparison expression node.
        ExprNode *comparison();

        // Parses and returns an arithmetic expression node.
        ExprNode *arith_expr();

        // Parses and returns a term expression node.
        ExprNode *term();

        // Parses and returns a factor expression node.
        ExprNode *factor();

        // Parses and returns an atom expression node.
        ExprNode *atom();

    private:
        // A reference to the Tokenizer object that contains the list of tokens to be parsed.
        Tokenizer &tokenizer;

        // Private function used to handle parse errors and output helpful error messages.
        void die(std::string where, std::string message, Token &token);

};


#endif //APYTHONINTERPRETER_PARSER_HPP
