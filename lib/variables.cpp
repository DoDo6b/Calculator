#include "variables.h"

bool Symbol_table::is_declared(std::string variable) const {    //is variable already declared?
    for(const Variable& var : var_table)
        if (var.name == variable) return true;  //seek out in symbol table for variable by name and return true in case of succes
    return false;
}

double Symbol_table::get_value(std::string name) const {    //get variable value by name from symbol table
    for(const Variable& var : var_table)
        if(var.name == name) return var.value;  //seek out in symbol table for variable by name and return value of variable
    throw std::runtime_error("undefined variable"); //throw exception if cant find variable in symbol table
}

double Symbol_table::set_value(std::string name, double value){ //set value by variable name in symbol table
    for(Variable& var : var_table)  
        if(var.name == name){   //seek out in symbol table for variable by name
            if(var.constant) throw std::runtime_error("Can't overwrite constant variable"); //throw exception if variable is constant(cant be redefined)
            var.value = value;  //set new value
            return value;
        }
    throw std::runtime_error("undefined variable"); //throw exception if cant find variable in symbol table
}

double Symbol_table::define_name(std::string name, double value, bool c){   //add new variable in to symbol table
    if(is_declared(name)) throw std::runtime_error("declared twice");   //throw exception if we are trying define variable twice
    var_table.push_back(Variable{name, value, c});  //add to the symbol table
    return value;
}

Symbol_table symbolt;