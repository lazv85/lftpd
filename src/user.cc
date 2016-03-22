#include <string>
#include "user.h"
#include "config.h"

User::User(std::string cmd, std::string param) : Command(cmd, param) {
    Config* cfg = Config::get_instance();
    
    std::string user_name = cfg->get_value("users",param);
    if(cfg->get_value("system","anonymous_access") == "yes" && user_name.empty()){
        this->status = USR_OK;
    }else{
        this->status = USR_NEED_PASSWORD;
    }
}

std::string User::get_response(){
    std::string str;
    
    if(this->status == USR_OK){
        str = std::string("230 Login successful");
    }else{
        str = std::string("331 user name is correct, password is required for ") + this->parameter;
    }
    
    return str;
}

USR_CODES User::get_status(){
    return this->status;
}