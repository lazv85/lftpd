#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include "command.h"

enum USR_CODES {USR_OK, USR_NEED_PASSWORD};

class User : public Command {
    private:
        USR_CODES status;
    public:
        User(std::string cmd, std::string param);
        std::string get_response();
        USR_CODES get_status();
};

#endif