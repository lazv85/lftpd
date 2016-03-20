#ifndef ABOR_H
#define ABOR_H

#include <iostream>
#include "command.h"

class Abor : public Command {
    public :
        Abor(std::string cmd, std::string param);
};
#endif