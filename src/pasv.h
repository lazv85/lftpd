#ifndef PASV_H
#define PASV_H

#include "command.h"
#include <iostream>

class Pasv : public Command {
    public:
        Pasv(std::string cmd, std::string param);
};

#endif