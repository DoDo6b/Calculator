#pragma once
#include <string>
#include <iostream>

class Token{
public:
    char kind;
    double value;
    std::string name;

    Token(char k) : kind{k}, value{0} { }
    Token(char k, double v) : kind{k}, value{v} { }
    Token(char k, std::string n) : kind{k}, value{0}, name{n} { }
};

class Token_stream{
public:
    Token get();
    void putback(Token t);
    void ignore(char c);
private:
    bool exists {false};
    Token buffer {'0'};
};

Token_stream tstream;