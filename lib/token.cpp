#include "token.h"
#include "namespace.h"

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
    buffer = token; //put token into buffer
    exists = true;  //now buffer is full
};

Token Token_stream::get(){
    if(exists){ //if buffer contains something we will take it from the buffer
        exists = false; //buffer is now empty
        return buffer;
    }

    char ch;
    std::cin.get(ch);   //get the first symbol from cin

    while (isspace(ch) && ch != '\n') std::cin.get(ch); //get the next one symbol if previous was space of endl

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
            return Token {ch};  //each token represents itself
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                std::cin.putback(ch);   //put symbol back into cin
                double val;
                std::cin >> val;    //read whole number in one time
                return Token {number, val};  
            }
        default:
            if(isalpha(ch)){
                std::string s;
                s += ch;
                while(std::cin.get(ch) && ((isalpha(ch) || isdigit(ch) || ch == '_')))
                    s += ch;    //concatenate the string
                std::cin.putback(ch);
                if(s == declkey) return Token{let}; //defining token kind
                else if(s == sqrtkey) return Token{square_root};
                else if(s == expkey) return Token{exponent};
                else if(s == sinkey) return Token{c_sin};
                else if(s == coskey) return Token{c_cos};
                else if(s == quitkey) return Token{quit};
                else if(s == helpkey) return Token{help};
                else return Token{name, s};
            }
            throw std::runtime_error("Bad token");
    }
};

Token_stream tstream;