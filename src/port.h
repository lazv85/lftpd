#ifndef PORT_H
#define PORT_H

#include "command.h"
#include <iostream>

enum PORT_CODES{PORT_OPEN, PORT_CLOSED,PORT_CANNOTBEOPEN};

class Port : public Command{
    private:
        std::string ip_addr;
        int port;
        PORT_CODES status;
    public:
        Port(std::string cmd, std::string param);
        int get_socket();
        std::string get_response();
};
#endif