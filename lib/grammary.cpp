#include "grammary.h"

double expression();    //forward declaration for primary

double calc_sqrt(){
    char ch;
    if(std::cin.get(ch) && ch != '(') throw std::runtime_error("'(' expected");
    std::cin.putback(ch);
    double expr = expression();
    if(expr < 0) throw std::runtime_error("sqrt: val < 0");
    return sqrt(expr);
}

double calc_pow(){
    Token token = tstream.get();
    if(token.kind != '(') throw std::runtime_error("'(' expected");
    double base = expression();
    token = tstream.get();
    if(token.kind != ',') throw std::runtime_error("',' =");
    double power = expression();
    token = tstream.get();
    if(token.kind != ')') throw std::runtime_error("')' expected");
    return pow(base, power);
}

double calc_sin(){
    char ch;
    if(std::cin.get(ch) && ch != '(') throw std::runtime_error("'(' expected");
    std::cin.putback(ch);
    double expr = expression();
    if(expr == 0 || expr == 180) return 0;  //return 0
    return sin(expr*M_PI/180);
}

double calc_cos(){
    char ch;
    if(std::cin.get(ch) && ch != '(') throw std::runtime_error("'(' expected");
    std::cin.putback(ch);
    double expr = expression();
    if(expr == 90 || expr == 270) return 0; //return 0 instead of 8.766e-11
    return cos(expr*M_PI/180);
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
//numbers and parenthesis
double primary(){
    Token token = tstream.get();
    switch(token.kind){
        case '(':
            {
                double expr = expression();
                token = tstream.get();
                if(token.kind != ')') throw std::runtime_error("')' expected");
                return expr;
            }
        case '{':
            {
                double expr = expression();
                token = tstream.get();
                if (token.kind != '}') throw std::runtime_error("'}' expected");
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
            throw std::runtime_error("primary expected");
    }
}
// factorial
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
// * and /
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
                    if (stepback == 0) throw std::runtime_error("division by zero");
                    lvalue /= stepback;
                    token = tstream.get();
                    break;
                }
            case '%':
                {
                    double stepback = secondary();
                    if (stepback == 0) throw std::runtime_error("division by zero");
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
// + and -
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
//declare variable
double declaration(){
    Token token = tstream.get(); //get variable name
    if(token.kind != name) throw std::runtime_error("name expected in declaration");    //syntax check
    std::string var_name = token.name;

    Token token2 = tstream.get();
    if(token2.kind != '=') throw std::runtime_error("= missing in declaration");    //syntax check

    double val = expression();  //get variable value
    symbolt.define_name(var_name, val); //define variable
    return val;
}