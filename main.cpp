#include "lib/token.h"
#include "lib/namespace.h"
#include "lib/variables.h"
#include "lib/grammary.h"

double enterpoint(){
    Token token = tstream.get();
    switch(token.kind){
        case let:
            return declaration();
        default:
            tstream.putback(token);
            return expression();
    }
}

void print_help(){
    std::cout << "FALT Calculator Manual\n"
         << std::endl
         << "Enter any expression. Type ';' for print\n"
         << "Square root can be calculated with 'sqrt' or 'pow'\n"
         << "For variable declaration use: 'let'\n";
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
                std::cout << result << enterpoint() << std::endl;
            }
        }
        catch(std::exception& e){
            std::cerr << e.what() << std::endl;
            tstream.ignore(print);
        }
} 

int main()
try{
    symbolt.define_name("pi", M_PI*10, true);
    symbolt.define_name("e", M_E, true);      

    std::cout << "Enter expression (?/help for help)\n";

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