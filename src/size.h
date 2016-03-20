#ifndef SIZE_H
#define SIZE_H

#include "command.h"
#include <iostream>

class Size : public Command{
    public:
        Size(std::string cmd, std::string param);
};
#endif