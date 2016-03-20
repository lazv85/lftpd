#ifndef PASS_H
#define PASS_H

#include "command.h"
#include <iostream>

class Pass : public Command {
    public:
        Pass(std::string cmd, std::string param);
        std::string get_response();
};

#endif