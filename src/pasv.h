#ifndef PASV_H
#define PASV_H

#include "command.h"
#include <iostream>
#include <string>
enum PASV_CODES{PASV_OK};

class Pasv : public Command {
    private:
        int sockfd;
        int port1;
        int port2;
        std::string ip;
    public:
        Pasv(std::string cmd, std::string param);
        int get_socket(std::string str);
        std::string get_response();
        int get_data_socket(int sockfd);
};

#endif