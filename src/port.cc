#include "port.h"
#include <boost/regex.hpp>
#include <sys/socket.h>
#include <iostream>
#include <cstdlib>
#include <netdb.h>
#include <cerrno>
#include "exception.h"

Port::Port(std::string cmd, std::string param) : Command(cmd, param) {
    boost::regex rgx("(\\d+,\\d+,\\d+,\\d+),(\\d+),(\\d+)");
    boost::smatch match;
    
    status = CANNOTBEOPEN;
    
    if (boost::regex_search(param, match, rgx)){
        std::string addr = match[1];
        std::string p1 = match[2];
        std::string p2 = match[3];
        this->ip_addr = boost::regex_replace(addr, boost::regex(","), std::string("."));
        this->port = atoi(p1.c_str())*256 + atoi(p2.c_str()) ;
        status = CLOSED;
    }
    
}

int Port::get_socket(){
    int server_socket = -1;
    
    if(status == CANNOTBEOPEN){
        return server_socket;
    }

    struct timeval timeout;      
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    
    struct hostent *server;
    struct sockaddr_in serv_addr;
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(server_socket < 0){
        status = CLOSED;
        return -1;
    }
    
    server = gethostbyname(this->ip_addr.c_str());
    
    if(server == NULL){
        status = CLOSED;
        close(server_socket);
        return -1;
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(this->port);
    
    if(setsockopt(server_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout))<0){
        status = CLOSED;
        close(server_socket);
        return -1;
    }
    
    if (connect(server_socket,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        status = CLOSED;
        close(server_socket);
        return -1;
    }
    
    status = OPEN;
    
    return server_socket;
}

std::string Port::get_response(){
    std::string str;
    
    switch(status){
        case OPEN: 
            str = std::string("225 Data connection open; no transfer in progress.");
            break;
        case CLOSED:
            str = std::string("425 Can't open data connection.") ;
            break;
        case CANNOTBEOPEN: 
            str = std::string("425 Can't open data connection.") ;
            break;
    }
    
    return str;
}