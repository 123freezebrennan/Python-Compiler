//
// Created by Ali Kooshesh on 4/4/23.
//

#include <iostream>
#include "SymTab.hpp"

void print_debug(TypeDescriptor *desc) {
    // If desc is of datatype NumberDescriptor, this function
    // print the value that is stored in it. Otherwise,
    // it just returns.
    // Use dynamic_cast to down-cast, from TypeDescriptor to
    // NumberDescriptr.
    if (StringDescriptor* strDesc = dynamic_cast<StringDescriptor*>(desc)){
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


void SymTab::setValueFor(const std::string &vName, TypeDescriptor *value) {
    // Define a variable by setting its initial value.
    if(debug)
        std::cout << vName << " <- " << value << std::endl;

    symTab[vName] = value;
}

bool SymTab::isDefined(const std::string &vName) {
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor *SymTab::getValueFor(const std::string &vName) {
    if (!isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    if (debug)
        std::cout << "SymTab::getValueFor: " << vName << " contains " << symTab.find(vName)->second << std::endl;
    if ((symTab.find(vName)->second)->type() == TypeDescriptor::INTEGER || (symTab.find(vName)->second)->type() == TypeDescriptor::DOUBLE)
        return dynamic_cast<NumberDescriptor *> (symTab.find(vName)->second);

    else
        return dynamic_cast<StringDescriptor *> (symTab.find(vName)->second);
    return nullptr;
}



void SymTab::print() {
    for(auto [var, value] : symTab ){
        std::cout << var << " = ";
        print_debug(value);
    }
}



