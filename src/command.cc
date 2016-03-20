#include "command.h"
#include <string>

Command::Command(std::string cmd, std::string param){
    this->command = cmd;
    this->parameter = param;
}

std::string Command::get_response(){
    std::string str("202 unsupported command");
    return str;
}

bool Command::quit_connection(){
    return false;
}

std::string Command::get_command(){
    return this->command;
}

std::string Command::get_parameter(){
    return this->parameter;
}

std::string Command::not_looged_in(){
    std::string str("530 not logged in, authorization required");
    return str;
}

std::string Command::need_password(){
    std::string str("331 username is correct, password required");
    return str;
}

bool Command::is_data(){
    return false;
}

int Command::transfer_data(int socket){
    return 0;
}

std::string Command::transfer_status(){
    std::string str("125 Data connection already open; transfer starting.");
    return str;
}