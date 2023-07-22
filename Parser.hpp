//
// Created by Ali Kooshesh on 4/4/23.
//

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
        // takes Tokenizer as a parameter in the constructor and sets the private variable to it
        Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

        
        Statements *stmt();

        AssignmentStatement *assign_stmt();
        
        ForStatement *forStatement();

        ExprNode *arith_expr();

        ExprNode *term();

        ExprNode *atom();

        PrintStatement *print();
        
        // std::string id();

    private:

        Tokenizer &tokenizer;
        void die(std::string where, std::string message, Token &token);

};


#endif //APYTHONINTERPRETER_PARSER_HPP
