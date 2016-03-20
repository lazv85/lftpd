#ifndef NOOP_H
#define NOOP_H

#include "command.h"
#include <iostream>

class Noop : public Command{
public:
    Noop(std::string, std::string param);

};
#endif