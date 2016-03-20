#ifndef SYST_H
#define SYST_H

#include <iostream>
#include "command.h"

class Syst : public Command {
    public:
        Syst(std::string cmd, std::string param);
        std::string get_response();
};

#endif