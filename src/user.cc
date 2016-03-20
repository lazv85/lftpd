#include <string>
#include "user.h"

User::User(std::string cmd, std::string param) : Command(cmd, param) {
    if(param == "ftp" || param == "anonymous"){
        this->status = AUTHORIZED;
    }else{
        this->status = PASSWORD_REQUIRED;
    }
}

std::string User::get_response(){
    std::string str;
    
    if(this->status == AUTHORIZED){
        str = std::string("230 Login successful");
    }else{
        str = std::string("331 user name is correct, password is required for ") + this->parameter;
    }
    
    return str;
}

Auth User::get_status(){
    return this->status;
}