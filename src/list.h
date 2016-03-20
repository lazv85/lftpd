#ifndef LIST_H
#define LIST_H

#include "command.h"
#include <iostream>

enum ListStatus{TRANSMITTED, NOCONNECTION, NETWORKFAILURE, CANNOTREADDISK};

class List : public Command {
    private:
        ListStatus status;
    public:
        List(std::string cmd, std::string param);
        std::string get_response();
        bool is_data();
        int transfer_data(int socket);
};

#endif