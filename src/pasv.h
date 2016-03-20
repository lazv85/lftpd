#ifndef PASV_H
#define PASV_H

#include "command.h"
#include <iostream>

class Pasv : public Command {
    private:
        int server_socket;
    public:
        Pasv(std::string cmd, std::string param);
        int get_socket();
        std::string get_response();
};

#endif