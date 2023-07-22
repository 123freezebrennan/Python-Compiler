
#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

// exit method for Parser
void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

// Gets the tree structure for Expressions 
ExprNode *Parser::arith_expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -
    // also handles rel-expr, which follow a similar method

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();

    while (tok.isAdditionOperator() || tok.isSubtractionOperator() || tok.equalToOperator() || tok.notEqualToOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        
        p->left() = left;
        p->right() = term();
        left = p;

        tok = tokenizer.getToken();
    }


    tokenizer.ungetToken();
    // add if statements and set left to expressions based off of token type
    
    return left;
}

// Get the tree structure for terms
ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %
    // also handles rel-term as it follows the same structure

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = atom();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator() || tok.isLessThanOperator() || tok.isGreaterThanOperator() || tok.lessThanOrEqualToOperator() || tok.greaterThanOrEqualToOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = atom();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}

// create the tree structure for primary values
ExprNode *Parser::atom() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);

    else if( tok.isName() )
        return new Variable(tok);

    else if (tok.isOpenParen()) {
        ExprNode *p = arith_expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }

    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}

// Main components of Parser, will create a vector of statements to be used in SymTab

Statements *Parser::stmt() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.

    // This function parses the grammar rules:

    // <statement> -> <assignStatement> <statement>
    // <statement> -> Epsilon

    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();

    while (tok.isName() || tok.isKeyWord()) {
        // if token is 'print'
        if(tok.isPrint()){
            tokenizer.ungetToken();
            PrintStatement *printStmt = print();
            stmts->addStatement(printStmt);
        }
        // if For loop
        else if (tok.isFor()){
            tokenizer.ungetToken();
            ForStatement *forStmt = forStatement();
            stmts->addStatement(forStmt);
        }
        // if token is just a variable name
        else{
            tokenizer.ungetToken();
            AssignmentStatement *assignStmt = assign_stmt();
            stmts->addStatement(assignStmt);
        }

        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return stmts;
}


// Creates assignment statement to be declared in SymTab
AssignmentStatement *Parser::assign_stmt() {
    // Parses the following grammar rule
    //
    // <assign-stmtement> -> <id> = <expr>

    Token tok = tokenizer.getToken();

    if (!tok.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", tok);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = arith_expr();
    // Step 1: commented these lines out to remove usage of semicolons

    // Token tok = tokenizer.getToken();
    // if (!tok.isSemiColon())
    //     die("Parser::assignStatement", "Expected a semicolon, instead got", tok);

    return new AssignmentStatement(tok.getName(), rightHandSideExpr);


}

// Sets PrintStatement object in Statements vector to print out variable name if it exists
PrintStatement *Parser::print(){
    Token tok = tokenizer.getToken();
    if (!tok.isPrint())
        exit(1);
    Token id = tokenizer.getToken();
    if (!id.isName())
        die("Parser::print", "Expected a name token, instead got", id);
    return new PrintStatement(id.getName());
}

// Sets ForStatement object in Statements to handle all statements declared in the dynamic scope of the for loop
// alongside the assignment statements and rel-expr required to determine the For loop range
ForStatement *Parser::forStatement(){
    // Determines "for"
    Token tok = tokenizer.getToken();

    if (!tok.isFor()){
        exit(1);
    }

    Token openParen = tokenizer.getToken();
    // determines "("
    if (!openParen.isOpenParen()){

        std::cout << "Parser::forStatement - No open parentheses found.\n";
        exit(1);

    }

    // Determines if there is assignStatement
    // add name string
    AssignmentStatement* prime_1 = assign_stmt();

    // Determines if there is a semicolon
    Token semicolon = tokenizer.getToken();

    if (! semicolon.isSemiColon()){
        std::cout << "Parser::forStatement - No semi colon found.\n";
        exit(1);
    }

    // Determines the relational expression
    ExprNode* relExpr = arith_expr();
    
    // Determines if there is a semicolon after

    semicolon = tokenizer.getToken();

    if (! semicolon.isSemiColon()){
        std::cout << "Parser::forStatement - No semi colon found.\n";
        exit(1);
    }


    // assignment statement after semicolon
    AssignmentStatement* prime_2 = assign_stmt();

    // if paren is closed
    Token closeParen = tokenizer.getToken();

    if (!closeParen.isCloseParen()){
        std::cout << "Parser::forStatement - No close parentheses found.\n";
        exit(1);
    }

    // if braces occur
    Token openBraces = tokenizer.getToken();
    if (!openBraces.isOpenBraces()){
        std::cout << "Parser::forStatement - No open braces found.\n";
        exit(1);
    }

    // recursive statement call to go through inner, symtab will handle the rest
    Statements *stmts = stmt();
    
    Token closeCurly = tokenizer.getToken();
    
        // while ()
        // if curly closes
    
    if (!closeCurly.isCloseBraces()){
        std::cout << "Parser::forStatement - No close braces found.\n";
        exit(1);
    }
    // Holds all needed values including statements, which will recursively call itself into the SymTab with dynamic scoping.
    return new ForStatement(prime_1, prime_2, relExpr, stmts);
}

