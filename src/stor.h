#ifndef STOR_H
#define STOR_H

#include <iostream>
#include "command.h"

class Stor : public Command {
    public:
        Stor(std::string cmd, std::string param);
};

#endif