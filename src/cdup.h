#ifndef CDUP_H
#define CDUP_H

#include "command.h"
#include <iostream>

class Cdup : public Command {
    public:
        Cdup(std::string cmd, std::string param);
};

#endif
