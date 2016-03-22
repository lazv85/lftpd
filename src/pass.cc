#include "pass.h"
#include <boost/regex.hpp>

Pass::Pass(std::string cmd, std::string param) : Command(cmd, param) {
    
}

std::string Pass::get_response(){
    std::string str;
    if(status == PASS_OK){
        str = std::string("230 Login successful");
    }else{
        str = std::string("430 Invalid user name or password");
    }
    
    return str;
}

PASS_CODES Pass::authorize(std::string user_settings){
    boost::regex rgx("([^:]+):([^:]+):([^:]+)");
    boost::smatch match;
    
    std::string password;
    
    if (boost::regex_search(user_settings, match, rgx)){
        password = match[1];
    }
    
    if(this->get_parameter() == password){
        status = PASS_OK;
        return PASS_OK;
    }else{
        status = PASS_WRONG_PASSWORD;
        return PASS_WRONG_PASSWORD;
    }
}

