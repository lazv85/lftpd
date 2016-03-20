#ifndef REIN_H
#define REIN_H

#include "command.h"
#include <iostream>

class Rein : public Command {
  public:
    Rein(std::string cmd, std::string param);
};
#endif 