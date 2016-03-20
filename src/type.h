#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include "command.h"

class Type : public Command{
  public:
    Type(std::string cmd, std::string param);
};

#endif