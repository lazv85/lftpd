#ifndef RETR_H
#define RETR_H

#include "command.h"
#include <iostream>

class Retr : public Command {
  public:
    Retr(std::string cmd, std::string param);
};

#endif