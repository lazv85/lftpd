#ifndef MKD_H
#define MKD_H

#include "command.h"
#include <iostream>

class Mkd : public Command{
  public:
    Mkd(std::string cmd, std::string param);
};

#endif