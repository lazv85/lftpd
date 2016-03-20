#ifndef MDTM_H
#define MDTM_H

#include "command.h"
#include <iostream>

class Mdtm : public Command {
  public:
    Mdtm(std::string cmd, std::string param);
};

#endif