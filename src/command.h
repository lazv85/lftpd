#ifndef COMMAND_H
#define COMMAND_H
#include <iostream>

class Command{
    protected:
        std::string command;
        std::string parameter;
    public:
        Command(std::string cmd, std::string param);
        virtual std::string get_response();
        virtual bool quit_connection();
        std::string get_command();
        std::string get_parameter();
        std::string not_looged_in();
        std::string need_password();
        virtual bool is_data();
        virtual int transfer_data(int socket);
        virtual std::string transfer_status();
};

#endif