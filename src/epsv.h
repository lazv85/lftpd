#ifndef EPSV_H
#define EPSV_H

#include "command.h"
#include <iostream>

class Epsv : public Command {
    public:
        Epsv(std::string cmd, std::string param);
};

#endif
