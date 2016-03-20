#include "quit.h"
#include <string>

Quit::Quit(std::string cmd,std::string param) : Command(cmd,param) {
    
}

bool Quit::quit_connection(){
    return true;
}

std::string Quit::get_response(){
    std::string str ;
    str = std::string("231 Good bye") ;
    return str;
}