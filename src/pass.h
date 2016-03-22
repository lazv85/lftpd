#ifndef PASS_H
#define PASS_H

#include "command.h"
#include <iostream>

enum PASS_CODES{PASS_OK, PASS_WRONG_PASSWORD};

class Pass : public Command {
    private:
        PASS_CODES status;
    public:
        Pass(std::string cmd, std::string param);
        std::string get_response();
        PASS_CODES authorize(std::string user_settings);
};

#endif