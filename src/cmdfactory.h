#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include <iostream>
#include "command.h"

class CommandFactory{
  private:
    std::string command;
    std::string parameters;
    int set_command_details(std::string cmd);
  public:
    Command * get_command(std::string cmd);
};

#endif