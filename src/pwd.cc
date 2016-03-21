#include "pwd.h"

Pwd::Pwd(std::string cmd, std::string param) : Command(cmd,param) {
    
}

std::string Pwd::get_response(){
    std::string str = std::string ("\"") + "/" +std::string ("\"");
    return str;
}