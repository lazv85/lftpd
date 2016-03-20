#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include "command.h"

enum Auth {AUTHORIZED, PASSWORD_REQUIRED};

class User : public Command {
    private:
        Auth status;
    public:
        User(std::string cmd, std::string param);
        std::string get_response();
        Auth get_status();
};

#endif