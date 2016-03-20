#ifndef DELE_H
#define DELE_H

#include "command.h"
#include <iostream>

class Dele : public Command {
    public:
        Dele(std::string cmd, std::string param);
};

#endif
