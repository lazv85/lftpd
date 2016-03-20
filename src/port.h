#ifndef PORT_H
#define PORT_H

#include "command.h"
#include <iostream>

enum PortStatus{OPEN, CLOSED,CANNOTBEOPEN};

class Port : public Command{
    private:
        std::string ip_addr;
        int port;
        PortStatus status;
    public:
        Port(std::string cmd, std::string param);
        int get_socket();
        std::string get_response();
};
#endif