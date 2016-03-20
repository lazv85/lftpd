#ifndef HELP_H
#define HELP_H

#include "command.h"
#include <iostream>

class Help : public Command {
    public:
        Help(std::string cmd, std::string param);
};

#endif
