#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <string>
struct Exception {
    std::string text;
    Exception(std::string p){
        text = p;
    }
    Exception(){}
};

#endif