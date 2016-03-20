#include "pasv.h"

Pasv::Pasv(std::string cmd, std::string param) : Command(cmd, param) {
    
}

std::string Pasv::get_response(){

    
}

int Pasv::get_socket(){
    return -1;
}