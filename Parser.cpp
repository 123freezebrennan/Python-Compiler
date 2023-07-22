
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
// create the tree structure for primary values
ExprNode *Parser::atom() {
    Token tok = tokenizer.getToken();
    if (tok.isOpenParen()) {
        ExprNode *p = test();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    else if (tok.isWholeNumber())
        return new WholeNumber(tok);
    else if (tok.isString())
        return new String(tok);
    else if( tok.isName() )
        return new Variable(tok);
    else if (tok.isDoubleNumber())
        return new DoubleNumber(tok);

    
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}

ExprNode *Parser::factor() {
    Token tok = tokenizer.getToken();
    if (tok.isSubtractionOperator()) {
        InfixExprNode* root = new InfixExprNode(tok);
        Token t = Token();
        root->left() = new Placeholder(t);
        root->right() = factor();
        return root;
    }
    else {
        tokenizer.ungetToken();
        return atom();
    }
}

// Get the tree structure for terms
ExprNode *Parser::term() {

    ExprNode *left = factor();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator() || tok.isIntDivision()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = factor();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}

// Gets the tree structure for Expressions 
ExprNode *Parser::arith_expr() {

    ExprNode *left = term();
    Token tok = tokenizer.getToken();

    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        
        p->left() = left;
        
        p->right() = term();

        left = p;

        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();

    return left;
}

ExprNode *Parser::comparison() {
    ExprNode *left = arith_expr();

    Token tok = tokenizer.getToken();

    while (tok.isRelationalOperator()){
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = arith_expr();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();

    return left; 
}

ExprNode* Parser::not_test() {
    Token tok = tokenizer.getToken();
    if (tok.isNot()) {
        InfixExprNode* root = new InfixExprNode(tok);
        Token t = Token();
        root->left() = new Placeholder(t);
        root->right() = not_test();
        return root;
    }
    else {
        tokenizer.ungetToken();
        return comparison();
    }
}

ExprNode *Parser::and_test() {
    ExprNode *left = not_test();
    Token tok = tokenizer.getToken();

    while (tok.isAnd()){
        InfixExprNode *p = new InfixExprNode(tok);

        p->left() = left;

        p->right() = not_test();

        left = p;

        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::or_test() {
    // needs to be and_test
    ExprNode *left = and_test();
    Token tok = tokenizer.getToken();

    while (tok.isOr()){
        InfixExprNode *p = new InfixExprNode(tok);

        // needs to be _and_test
        p->left() = left;

        // needs to be and_test
        p->right() = and_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::test() {
    return or_test();
}

// Sets PrintStatement object in Statements vector to print out variable name if it exists
std::vector<ExprNode*> Parser::testlist() {
    std::vector<ExprNode*> inside_print;

    Token id = tokenizer.getToken();
    while (!id.isCloseParen() && !id.eol() && !id.eof()){
        if (id.isComma()){
            inside_print.push_back(new String(id));
        }
        else{
            tokenizer.ungetToken();
            inside_print.push_back(test());
        }
        id = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return inside_print;
}

PrintStatement *Parser::print_stmt() {
    Token tok = tokenizer.getToken();
    if (!tok.isPrint())
        die("Parser::simple_stmt::print_stmt", "Expected an 'print token, instead got", tok);
    
    
    Token openParen = tokenizer.getToken();
    if (!openParen.isOpenParen()){
        std::cout << "Parser::print - No open parentheses found.\n";
        exit(1);
    }
    
    std::vector<ExprNode*> inside_print = testlist();
    Token id = tokenizer.getToken();
    if (!id.isCloseParen())
        die("Parser::print_stmt", "Expected a closed parentheses, instead got", id);
    

    return new PrintStatement(inside_print);
}

// Creates assignment statement to be declared in SymTab
AssignmentStatement *Parser::assign_stmt() {


    Token tok = tokenizer.getToken();

    if (!tok.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", tok);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = test();

    return new AssignmentStatement(tok.getName(), rightHandSideExpr);
}

// Main components of Parser, will create a vector of statements to be used in SymTab
Statement *Parser::simple_stmt() {
    Token tok = tokenizer.getToken();

    if(tok.isPrint()) {
        tokenizer.ungetToken();
        PrintStatement *printStmt = print_stmt();
        Token is_nl = tokenizer.getToken();

        if (!is_nl.eol() && !is_nl.eof() && !is_nl.isComment()){
            die("Parser::simple_stmt::print", "Expected an eol token, instead got", is_nl);
        }
        return printStmt;
    }
    else {
        tokenizer.ungetToken();
        AssignmentStatement *assignStmt = assign_stmt();
        Token is_nl = tokenizer.getToken();

        if (!is_nl.eol() && !is_nl.eof() && !is_nl.isComment()){
            die("Parser::simple_stmt::assign_stmt", "Expected an eol token, instead got", is_nl);
        }
        return assignStmt;
    }

}

Statements *Parser::suite() {
    int to_indent = tokenizer.getIndent();

    Token isNewLine = tokenizer.getToken();
    if (!isNewLine.eol() && !isNewLine.isComment()){
        std::cout << "Parser::suite - No new line found.\n";
        exit(1);
    }
    tokenizer.getToken();
    if (tokenizer.getIndent() < to_indent){
        std::cout << "Parser::suite - Bad indentation.\n";
        exit(1);  
    }

    tokenizer.ungetToken();

    Statements *stmts = stmt();
    if (stmts->getAmountOfStatements() < 1){
        std::cout << "Parser:: - Bad indentation.\n";
        exit(1);
    }
    return stmts;
}

// Sets ForStatement object in Statements to handle all statements declared in the dynamic scope of the for loop
// alongside the assignment statements and rel-expr required to determine the For loop range
ForStatement *Parser::forStatement() {
    // Determines "for"
    Token tok = tokenizer.getToken();

    if (!tok.isFor()){
        std::cout << "Parser::forStatement - No 'for' found.\n";
        exit(1);
    }

    Token name = tokenizer.getToken();
    if (!name.isName()){
        std::cout << "Parser::forStatement - No 'variable' found.\n";
        exit(1);     
    }
    Token isIn = tokenizer.getToken();
    if (!isIn.isKeyWord() && !isIn.isIn()){
           std::cout << "Parser::forStatement - No 'in' found.\n";
        exit(1);     
    }
    Token isRange = tokenizer.getToken();
    if (!isRange.isKeyWord() && !isRange.isRange()){
           std::cout << "Parser::forStatement - No 'range' found.\n";
        exit(1);     
    }
    Token openParen = tokenizer.getToken();
    // determines "("
    if (!openParen.isOpenParen()){

        std::cout << "Parser::forStatement - No open parentheses found.\n";
        exit(1);

    }
    int range_value;
    Token is_integer = tokenizer.getToken();
    if (!is_integer.isWholeNumber()){
        std::cout << "Parser::forStatement - No whole number 1 found.\n";
        exit(1);
    }
    range_value = is_integer.getWholeNumber();

    bool was_end = false;
    int end = 0;
    Token first_comma = tokenizer.getToken();
    
    if (first_comma.isComma()){
        was_end = true;
        Token is_end_int = tokenizer.getToken();

        if (is_end_int.isSubtractionOperator()){
            is_end_int = tokenizer.getToken();

            if (!is_end_int.isWholeNumber()){
                std::cout << "Parser::forStatement - No whole number 2 found.\n";
                exit(1);
            }

            end = is_end_int.getWholeNumber() * -1;       
        }
        else {

            if (!is_end_int.isWholeNumber()){
                std::cout << "Parser::forStatement - No whole number 2 found.\n";
                exit(1);
            }
            end = is_end_int.getWholeNumber();
        }
        
    }
    else
        tokenizer.ungetToken();
    

    bool was_step = false;
    int step = 0;
    Token second_comma = tokenizer.getToken();

    if (second_comma.isComma() && was_end) {
        was_step = true;
        Token is_end_int = tokenizer.getToken();

        if (is_end_int.isSubtractionOperator()){
            is_end_int = tokenizer.getToken();
            if (!is_end_int.isWholeNumber()){
                std::cout << "Parser::forStatement - No whole number 3 found.\n";
                exit(1);
            }
            step = is_end_int.getWholeNumber() * -1;       
        }
        else {
            if (!is_end_int.isWholeNumber()){
                std::cout << "Parser::forStatement - No whole number 3 found.\n";
                exit(1);
            }
            step = is_end_int.getWholeNumber();
        }
    }
    else
        tokenizer.ungetToken();
    
    Range* range;

    if (!was_end && !was_step)
         range = new Range(range_value);
    else if (was_end && !was_step)
        range = new Range(range_value, end);
    else 
        range = new Range(range_value, end, step);

    // if paren is closed
    Token closeParen = tokenizer.getToken();

    if (!closeParen.isCloseParen()){
        std::cout << "Parser::forStatement - No close parentheses found.\n";
        exit(1);
    }

    // if braces occur
    Token isColon = tokenizer.getToken();
    if (!isColon.isColon()){
        std::cout << "Parser::forStatement - No colon found.\n";
        exit(1);
    }

    // recursive statement call to go through inner, symtab will handle the rest
    Statements *stmts = suite();
    // Holds all needed values including statements, which will recursively call itself into the SymTab with dynamic scoping.
    return new ForStatement(name.getName(), range, stmts);
}

IfStatement *Parser::ifStatement(){
    Token tok = tokenizer.getToken();
    if (!tok.isIf()){
        std::cout << "Parser::ifStatement - No 'if' found.\n";
        exit(1);
    }
    ExprNode* if_test = test();

    tok = tokenizer.getToken();
    if (!tok.isColon()){
        std::cout << "Parser::ifStatement - No ':' found.\n";
        exit(1);
    }

    Statements *suite_if = suite();

    std::vector<ExprNode*> test_elifs;
    std::vector<Statements*> suite_elifs;
    Token is_elif = tokenizer.getToken();
    if (is_elif.isElif()){
        while (is_elif.isElif()) {
            ExprNode* elif_test = test();
            test_elifs.push_back(elif_test);
            
            tok = tokenizer.getToken();
            if (!tok.isColon()){
                std::cout << "Parser::elifStatement - No ':' found.\n";
                exit(1);
            }

            Statements *suite_elif = suite();

            suite_elifs.push_back(suite_elif);
            is_elif = tokenizer.getToken();
        }
    }
    else {
        tokenizer.ungetToken();
    }

    tokenizer.ungetToken();
    bool is_elifs = false;
    if (suite_elifs.size() > 0){
        is_elifs = true;
    }

    Statements *suite_else;
    bool is_else = false;
    Token is_elses = tokenizer.getToken();
    if (is_elses.isElse()) {
        Token is_colon = tokenizer.getToken();
        if (!is_colon.isColon()){
            std::cout << "Parser::elseStatement - No ':' found.\n";
            exit(1);
        }
        suite_else = suite();
        
        is_else = true;
    }
    else {
        tokenizer.ungetToken();
    }
    
    
    return new IfStatement(if_test, suite_if, test_elifs, suite_elifs, is_elifs, suite_else, is_else);
}

Statement *Parser::compound_stmt() {
    Token tok = tokenizer.getToken();
    if (tok.isFor()) {
        tokenizer.ungetToken();
        ForStatement *forStmt = forStatement();
        return forStmt;
    }
    else if (tok.isIf()) {
        tokenizer.ungetToken();
        IfStatement *ifStmt = ifStatement();
        return ifStmt;
    }
    return nullptr;

}

Statements *Parser::stmt() {

    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    int base_indent = tokenizer.getIndent();
    while ((tok.isName() || tok.isKeyWord() || tok.eol() || tok.isComment()) && base_indent == tokenizer.getIndent()) {
        // if token is 'print'
        if(tok.isPrint() || tok.isName()){

            tokenizer.ungetToken();
            stmts->addStatement(simple_stmt());
        }
        // if For loop
        else if (tok.isFor() || tok.isIf()){
            tokenizer.ungetToken();
            stmts->addStatement(compound_stmt());
        }
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return stmts;
}