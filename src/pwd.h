#ifndef PWD_H
#define PWD_H

#include "command.h"
#include <iostream>

class Pwd : public Command {
  public:
    Pwd(std::string cmd, std::string param);
};

#endif