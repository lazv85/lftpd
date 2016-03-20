#include "pass.h"

Pass::Pass(std::string cmd, std::string param) : Command(cmd, param) {
    
}

std::string Pass::get_response(){
    std::string str("230 Login successful");
    return str;
}

