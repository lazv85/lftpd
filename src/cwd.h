#ifndef CWD_H
#define CWD_H

#include "command.h"
#include <iostream>

class Cwd : public Command {
    public:
        Cwd(std::string cmd, std::string param);
};

#endif
