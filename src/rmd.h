#ifndef RMD_H
#define RMD_H

#include <iostream>
#include "command.h"

class Rmd : public Command {
  public:
    Rmd(std::string cmd, std::string param);
};
#endif