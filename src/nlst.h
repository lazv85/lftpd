#ifndef NLST_H
#define NLS_H

#include "command.h"
#include <iostream>

class Nlst : public Command {
  public:
    Nlst(std::string, std::string param);
};

#endif