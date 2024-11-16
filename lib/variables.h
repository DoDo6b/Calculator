#pragma once
#include <vector>

class Variable{
public:
    std::string name;   //variable name
    double value;   //variable value
    bool constant;  //constant or variable
    Variable(std::string n, double v, bool c = false)
        : name{n}, value{v}, constant{c} { }
};

class Symbol_table{
    std::vector<Variable> var_table;    //contains all variables
public:
    bool is_declared(std::string) const;    //is variable already declared?
    double get_value(std::string) const;    //get variable value by name from symbol table
    double set_value(std::string, double);  //set value by variable name in symbol table
    double define_name(std::string, double, bool con = false);  //add new variable in to symbol table
};

extern Symbol_table symbolt;