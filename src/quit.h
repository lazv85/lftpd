#ifndef QUIT_H
#define QUIT_H

#include "command.h"
#include <iostream>

class Quit : public Command {
    public:
        Quit(std::string cmd, std::string param);
        bool quit_connection();
        std::string get_response();
};

#endif