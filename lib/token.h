#pragma once
#include <string>
#include <iostream>

class Token{
public:
    char kind;  //kind of token
    double value;   //token containment(doesnt exist if not a number)
    std::string name;   //variable name(doesnt exist if it not a variable)

    Token(char k) : kind{k} { }
    Token(char k, double v) : kind{k}, value{v} { }
    Token(char k, std::string n) : kind{k}, value{0}, name{n} { }
};

class Token_stream{
public:
    Token get();    //get a token from buffer or cin(if buffer is empty)
    void putback(Token t);  //put token into buffer
    void ignore(char c);    //cleaning buffer
private:
    bool exists {false};    //buffer contains something
    Token buffer {'0'}; //buffer container
};

extern Token_stream tstream;