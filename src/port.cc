#include "port.h"
#include <boost/regex.hpp>
#include <sys/socket.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include "sysutil.h"
#include "exception.h"

Port::Port(std::string cmd, std::string param) : Command(cmd, param) {
    boost::regex rgx("(\\d+,\\d+,\\d+,\\d+),(\\d+),(\\d+)");
    boost::smatch match;
    
    status = PORT_CANNOTBEOPEN;
    
    if (boost::regex_search(param, match, rgx)){
        std::string addr = match[1];
        std::string p1 = match[2];
        std::string p2 = match[3];
        this->ip_addr = boost::regex_replace(addr, boost::regex(","), std::string("."));
        this->port = atoi(p1.c_str())*256 + atoi(p2.c_str()) ;
        status = PORT_CLOSED;
    }
    
}

int Port::get_socket(){
    if(status == PORT_CANNOTBEOPEN){
        return -1;
    }
    
    SysUtil *p_sysutil = SysUtil::get_instance();
    
    int sockfd = p_sysutil->connect_to_server(this->ip_addr,this->port);
    
    if(sockfd < 0){
        return -1;
    }else{
        status = PORT_OPEN;
    }
    
    return sockfd;
}

std::string Port::get_response(){
    std::string str;
    
    switch(status){
        case PORT_OPEN: 
            str = std::string("225 Data connection open; no transfer in progress.");
            break;
        case PORT_CLOSED:
            str = std::string("425 Can't open data connection.") ;
            break;
        case PORT_CANNOTBEOPEN: 
            str = std::string("425 Can't open data connection.") ;
            break;
    }
    
    return str;
}