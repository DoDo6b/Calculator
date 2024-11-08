#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

const char number = '8';
const char quit = 'q';
const char print = ';';
const char name = 'a';
const char let = 'L';
const char help = '?';
const char c_sin = 's';
const char c_cos = 'c';

const std::string prompt = "> ";
const std::string result = "= ";
const std::string declkey = "let";

const char square_root = '@';
const char exponent = '^';
const std::string sqrtkey = "sqrt";
const std::string expkey = "pow";
const std::string sinkey = "sin";
const std::string coskey = "cos";
const std::string quitkey = "quit";
const std::string helpkey = "help";

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

void Token_stream::ignore(char c){
    if(exists && c == buffer.kind){
        exists = false;
        return;
    }
    exists = false;

    char ch = 0;
    while(std::cin >> ch)
        if(ch == c) return;
}

void Token_stream::putback(Token token){
    buffer = token;
    exists = true;
};

Token Token_stream::get(){
    if(exists){
        exists = false;
        return buffer;
    }

    char ch;
    std::cin.get(ch);

    while (isspace(ch) && ch != '\n') std::cin.get(ch);

    switch(ch){
        case '\n':
            return Token{print};
        case print:
        case quit:
        case help:
        case '(':
        case ')':
        case '{':
        case '}':
        case '!':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case ',':
            return Token { ch };
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                std::cin.putback(ch);
                double val;
                std::cin >> val;
                return Token { number, val };  
            }
        default:
            if(isalpha(ch)){
                std::string s;
                s += ch;
                while(std::cin.get(ch) && ((isalpha(ch) || isdigit(ch) || ch == '_')))
                    s += ch;
                std::cin.putback(ch);
                if(s == declkey) return Token{let};
                else if(s == sqrtkey) return Token{square_root};
                else if(s == expkey) return Token{exponent};
                else if(s == sinkey) return Token{c_sin};
                else if(s == coskey) return Token{c_cos};
                else if(s == quitkey) return Token{quit};
                else if(s == helpkey) return Token{help};
                else return Token{name, s};
            }
            std::runtime_error("Bad token");
    }
};

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

bool Symbol_table::is_declared(std::string variable){
    for(const Variable& var : var_table)
        if (var.name == variable) return true;
    return false;
}

double Symbol_table::get_value(std::string name){
    for(const Variable& var : var_table)
        if(var.name == name) return var.value;
    std::runtime_error("get: undefined variable");
}

double Symbol_table::set_value(std::string name, double value){
    for(Variable& var : var_table)
        if(var.name == name){
            if(var.constant) std::runtime_error("Can't overwrite constant variable");
            var.value = value;
            return value;
        }
    std::runtime_error("set: undefined variable");
}

double Symbol_table::define_name(std::string name, double value, bool con){
    if(is_declared(name)) std::runtime_error("declared twice");
    var_table.push_back(Variable{name, value, con});
    return value;
}

Symbol_table symbolt;

Token_stream tstream;

double expression();

double calc_sqrt(){
    char ch;
    if(std::cin.get(ch) && ch != '(') std::runtime_error("'(' expected");
    std::cin.putback(ch);
    double expr = expression();
    if(expr < 0) std::runtime_error("sqrt: negative val is imaginary");
    return sqrt(expr);
}

double calc_pow(){
    Token token = tstream.get();
    if(token.kind != '(') std::runtime_error("'(' expected");
    double base = expression();
    token = tstream.get();
    if(token.kind != ',') std::runtime_error("',' =");
    double power = expression();
    token = tstream.get();
    if(token.kind != ')') std::runtime_error("')' expected");
    return pow(base, power);
}

double calc_sin(){
    char ch;
    if(std::cin.get(ch) && ch != '(') std::runtime_error("'(' expected");
    std::cin.putback(ch);
    double expr = expression();
    if(expr == 0 || expr == 180) return 0;
    return sin(expr*M_1_PI/18);
}

double calc_cos(){
    char ch;
    if(std::cin.get(ch) && ch != '(') std::runtime_error("'(' expected");
    std::cin.putback(ch);
    double expr = expression();
    if(expr == 90 || expr == 270) return 0;
    return cos(expr*M_1_PI/18);
}

double handle_variable(Token& token){
    Token token2 = tstream.get();
    if(token2.kind == '=')
        return symbolt.set_value(token.name, expression());
    else{
        tstream.putback(token2);
        return symbolt.get_value(token.name);
    }
}

double primary(){
    Token token = tstream.get();
    switch(token.kind){
        case '(':
            {
                double expr = expression();
                token = tstream.get();
                if(token.kind != ')') std::runtime_error("')' expected");
                return expr;
            }
        case '{':
            {
                double expr = expression();
                token = tstream.get();
                if (token.kind != '}') std::runtime_error("'}' expected");
                return expr;
            }
        case number: 
            return token.value;
        case name:
            return handle_variable(token);
        case '-':
            return -primary();
        case '+':
            return primary();
        case square_root:
            return calc_sqrt();
        case exponent:
            return calc_pow();
        case c_sin:
            return calc_sin();
        case c_cos:
            return calc_cos();
        default:
            std::runtime_error("primary expected");
    }
}

double secondary(){
    double lvalue = primary();
    Token token = tstream.get();

    while(true){
        switch(token.kind){
            case '!':
                if (lvalue == 0) return 1;
                for (int i = lvalue - 1; i > 0; --i) lvalue *= i;
                token = tstream.get();
                break;
            default:
                tstream.putback(token);
                return lvalue;
        }
    }
}

double term(){
    double lvalue = secondary();
    Token token = tstream.get();

    while(true){
        switch(token.kind){
            case '*':
                lvalue *= secondary();
                token = tstream.get();
                break;
            case '/':
                {
                    double stepback = secondary();
                    if (stepback == 0) std::runtime_error("divide by zero");
                    lvalue /= stepback;
                    token = tstream.get();
                    break;
                }
            case '%':
                {
                    double stepback = secondary();
                    if (stepback == 0) std::runtime_error("%: divide by zero");
                    lvalue = fmod(lvalue, stepback);
                    token = tstream.get();
                    break;
                }
            default:
                tstream.putback(token); 
                return lvalue;
        }
    }
}

double expression(){
    double lvalue = term();
    Token token = tstream.get();

    while(true){
        switch(token.kind){
            case '+':
                lvalue += term();
                token = tstream.get();
                break;
            case '-':
                lvalue -= term();
                token = tstream.get();
                break;
            default:
                tstream.putback(token);
                return lvalue;
        }
    }
}

double definition(){
    Token token = tstream.get();
    if(token.kind != name) std::runtime_error("name expected in declaration");
    std::string var_name = token.name;

    Token token2 = tstream.get();
    if(token2.kind != '=') std::runtime_error("= missing in declaration");

    double expr = expression();
    symbolt.define_name(var_name, expr);
    return expr;
}

double declaration(){
    Token token = tstream.get();
    switch(token.kind){
        case let:
            return definition();
        default:
            tstream.putback(token);
            return expression();
    }
}

void print_help(){
    std::cout << "FALT Calculator Manual\n"
         << std::endl
         << "Enter any expression. Type ';' for \n"
         << "Square root can be calculated with 'sqrt' or 'pow'\n"
         << "For variable declaration use: 'let'\n";
}

void clean_up_mess(){
    tstream.ignore(print);
}

void calculate(){
    while(std::cin)
        try{
            std::cout << prompt;
            Token token = tstream.get();
            while(token.kind == print) token = tstream.get();
            if(token.kind == help) print_help();
            else if(token.kind == quit) return;
            else{
                tstream.putback(token);
                std::cout << result << declaration() << '\n';
            }
        }
        catch(std::exception& e){
            std::cerr << e.what() << '\n';
            clean_up_mess();
        }
} 

int main()
try{
    symbolt.define_name("pi", M_1_PI*10, true);
    symbolt.define_name("e", M_E, true);      

    std::cout << "Enter expression (? for help)\n";

    calculate();
    return 0;
}
catch(std::exception& e){
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
}
catch(...){
    std::cerr << "Unknown error\n";
    return 2;
}