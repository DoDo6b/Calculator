#include "lib/token.h"
#include "lib/namespace.h"
#include "lib/variables.h"
#include "lib/grammary.h"

double enterpoint(){
    Token token = tstream.get();    //get the token from buffer
    switch(token.kind){
        case let:   //declaring variable
            return declaration();
        default:
            tstream.putback(token); //put token into the buffer
            return expression();    //goto grammary
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
            std::cout << prompt;    //print prompt symbol(> by default)
            Token token = tstream.get();    //get the first one token
            while(token.kind == print) token = tstream.get();
            if(token.kind == help) print_help();    //print help if it needs
            else if(token.kind == quit) return; //exit point
            else{
                tstream.putback(token); //put token into buffer
                std::cout << result << enterpoint() << std::endl; //goto enterpoint
            }
        }
        catch(std::exception& e){
            std::cerr << e.what() << std::endl;
            tstream.ignore(print);  //cleaning cin if we have fault
        }
} 

int main()
try{
    symbolt.define_name("pi", M_PI, true);  //defining constant pi
    symbolt.define_name("e", M_E, true);    //defining constant e

    std::cout << "Enter expression (?/help for help)\n";

    calculate();    //start
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