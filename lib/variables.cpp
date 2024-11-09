#include "variables.h"

bool Symbol_table::is_declared(std::string variable){
    for(const Variable& var : var_table)
        if (var.name == variable) return true;
    return false;
}

double Symbol_table::get_value(std::string name){
    for(const Variable& var : var_table)
        if(var.name == name) return var.value;
    throw std::runtime_error("undefined variable");
}

double Symbol_table::set_value(std::string name, double value){
    for(Variable& var : var_table)
        if(var.name == name){
            if(var.constant) throw std::runtime_error("Can't overwrite constant variable");
            var.value = value;
            return value;
        }
    throw std::runtime_error("undefined variable");
}

double Symbol_table::define_name(std::string name, double value, bool c){
    if(is_declared(name)) throw std::runtime_error("declared twice");
    var_table.push_back(Variable{name, value, c});
    return value;
}

Symbol_table symbolt;