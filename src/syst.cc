#include "syst.h"

Syst::Syst(std::string cmd, std::string param) : Command(cmd, param) {
    
}

std::string Syst::get_response(){
    std::string str;
    
    #if defined(_AIX)
        str = std::string("215 AIX");
    #endif
    
    #if defined(__unix__)
        str = std::string("215 Unix");
    #endif
    
    #if defined(__hpux)
        str = std::string("215 HP-UX");
    #endif
    
     #if defined(__linux__)
        str = std::string("215 Linux");
    #endif
    
    #if defined(__APPLE__)
        str = std::string("215 OSX");
    #endif
    
    #if defined(__sun)
        str = std::string("215 Solaris");
    #endif
    
    #if defined(__CYGWIN__)
        str = std::string("215 Cygwin");
    #endif
    
    #if defined(_WIN32)
        str = std::string("215 Windows(32bit)");
    #endif
    
    
    #if defined(_WIN64)
        str = std::string("215 Windows(64bit)");
    #endif
    
    return str;
}