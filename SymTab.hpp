#ifndef APYTHONINTERPRETER_SYMTAB_HPP
#define APYTHONINTERPRETER_SYMTAB_HPP

#include <string>
#include <map>

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.
struct TypeDescriptor {
// The base class for datatype descriptors.
// This enum can expand to include additional types.
    enum types {INTEGER, DOUBLE, STRING};
    TypeDescriptor(types type): _type{type} {}
    types &type() { return _type; }
    virtual ~TypeDescriptor() {}

    private:
        types _type;
};

struct NumberDescriptor: public TypeDescriptor {
// Number descriptor includes integer, double, and boolean datatypes.
    NumberDescriptor(types descType): TypeDescriptor(descType) {}

    union {
        int intValue;
        double doubleValue;
    } value;

    private:
        types _type;
};
struct StringDescriptor : public TypeDescriptor {
    StringDescriptor(types descType) : TypeDescriptor(TypeDescriptor::STRING) {}

    std::string value;

    private:
        types _type;
};

class SymTab {

    public:
        void setValueFor(const std::string &vName, TypeDescriptor *value);
        bool isDefined(const std::string &vName);
        TypeDescriptor *getValueFor(const std::string &vName);
        void print();

    private:
        std::map<std::string, TypeDescriptor *> symTab;
        bool debug = false;

};



#endif //APYTHONINTERPRETER_SYMTAB_HPP
