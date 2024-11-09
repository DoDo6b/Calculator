#pragma once
#include <vector>

class Variable{
public:
    std::string name;
    double value;
    bool constant;
    Variable(std::string n, double v, bool c = false)
        : name{n}, value{v}, constant{c} { }
};

class Symbol_table{
    std::vector<Variable> var_table;
public:
    bool is_declared(std::string);
    double get_value(std::string);
    double set_value(std::string, double);
    double define_name(std::string, double, bool con = false);
};

extern Symbol_table symbolt;