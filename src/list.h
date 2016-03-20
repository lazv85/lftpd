#ifndef LIST_H
#define LIST_H

#include "command.h"
#include <iostream>
#include <vector>

enum ListStatus{TRANSMITTED, NOCONNECTION, NETWORKFAILURE, CANNOTREADDISK};

class List : public Command {
    private:
        ListStatus status;
        std::vector<std::string> list_directory(std::string dir_name);
    public:
        List(std::string cmd, std::string param);
        std::string get_response();
        bool is_data();
        int transfer_data(int socket);
        std::string transfer_status();
};

#endif