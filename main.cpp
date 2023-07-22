#include <iostream>
#include<vector>

// Next to checkout are these based off of need for other recursive calls
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"


/*
main()
    Takes in the file to interpret, sets the tokenizer,
    Parses through the tokens in tokenizer, and creates
    a vector of statements to then be determined by the SymTab
*/

int main(int argc, char *argv[]) {
    // Need input file
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        exit(1);
    }

    // open file from argc
    std::ifstream inputStream;

    inputStream.open(argv[1], std::ios::in);
    // exit if it cannot find file name
    if (!inputStream.is_open()) {
        std::cout << "Unable top open " << argv[1] << ". Terminating...";
        perror("Error when attempting to open the input file.");
        exit(2);
    }
    // Tokenizes the inputstream
    Tokenizer tokenizer(inputStream);
    // parse through tokenized object
    Parser parser(tokenizer);
    // get object from parser to set as a pointer to 'statements'
    Statements *statements = parser.stmt();
    // get last token value
    Token lastToken = tokenizer.getToken();
    // breaker to make sure there is an eof
    if (!lastToken.eof()) {
        std::cout << "Unexpected token in input." << std::endl;
        lastToken.print();
        std::cout << std::endl;
        exit(1);
    }
    // creation of the SymTab object

    SymTab symTab;
    // method call on statements print() (assuming it calls all prints possibly)
    // statements->print();
    // im guessing this declares an object to contruct the 'symtab' and then the statements get evaluated in 
    // evaluate 

    // its calling the address not making a blank statement. this is for dynamic scoping i believe
    statements->evaluate(symTab);
    // std::cout << std::endl << "Symbol table contains the following variables.\n";
    // print all variables from symTab
    // symTab.print();
    return 0;
}
