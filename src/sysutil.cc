#include "sysutil.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <iostream>
#include <sys/ioctl.h>
#include <cerrno>
#include <netinet/tcp.h>
#include <string>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "exception.h"

SysUtil * SysUtil::instance = NULL;

SysUtil::SysUtil(){
    waiting = true;
}

SysUtil* SysUtil::get_instance(){
    if(!SysUtil::instance){
        SysUtil::instance = new SysUtil();
    }
    return SysUtil::instance;
}

int SysUtil::init_server_socket(int portnum){
    int sockfd;
    
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    if(sockfd < 0){
        error_info = strerror(errno);
        return -1;
    }
    
    int flag = 1 ;
    
    sockaddr_in addr;
    bzero((char *) &addr, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(portnum);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (struct sockaddr *) &addr,sizeof(addr)) < 0){
        error_info = strerror(errno);
        return -1;
    }
    
    if(listen(sockfd,5) < 0){
        error_info = strerror(errno);
        return -1;
    }
    
    return sockfd;
}

int SysUtil::connect_to_server(std::string host, int port){
    
    int sockfd;
    struct timeval timeout;      
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    
    struct hostent *server;
    struct sockaddr_in serv_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sockfd < 0){
        error_info = strerror(errno);
        return -1;
    }
    
    server = gethostbyname(host.c_str());
    
    if(server == NULL){
        error_info = strerror(errno);
        close(sockfd);
        return -1;
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    
    if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout))<0){
        error_info = strerror(errno);
        close(sockfd);
        return -1;
    }
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        error_info = strerror(errno);
        close(sockfd);
        return -1;
    }
    
    return sockfd;
}

int SysUtil::wait_on_socket(int sockfd, int timeout_second){
    fd_set active_fd_set, read_fd_set;
    
    sockaddr_in addr;
    socklen_t size_addr = sizeof(addr);
    struct timeval timeout;
    
    int retval;
    
    do{
        FD_ZERO (&active_fd_set);
        FD_SET (sockfd, &active_fd_set);
            
        timeout.tv_sec = timeout_second;
        timeout.tv_usec = 0;
        retval = select(sockfd+1,&active_fd_set,NULL,NULL,&timeout);
            
    }while(retval <= 0 && waiting);
    
    return retval;
}

int SysUtil::get_client_socket(int sockfd, int timeout_second){

    int retval = 1;
    
    if(timeout_second > 0){
        retval = wait_on_socket(sockfd, timeout_second);
    }
    
    int client_socket = -1;
    
    if(retval > 0){
        sockaddr_in addr;
        socklen_t size_addr = sizeof(addr);
            
        client_socket = accept(sockfd,(struct sockaddr *) &addr,&size_addr);
            
        if(client_socket<0){
            error_info = strerror(errno);
            return -1;
        }
    }
    return client_socket;
}

int SysUtil::read_from_socket(int sockfd, std::string * str){
     int len;
            
    if(ioctl(sockfd,FIONREAD,&len)<0){
        error_info = strerror(errno);
        return -1;
    }
            
    char * buffer ;
    buffer = new char[len];
    bzero(buffer,len) ;
    
    if(read(sockfd,buffer,len)<0){
        error_info = strerror(errno);
        return -1;
    }

    *str = std::string(buffer);
    delete [] buffer;
    
    return 0;
}

int SysUtil::write_to_socket(int sockfd, std::string msg){
    std::string smsg = msg + "\r\n";
    const char * c_response = smsg.c_str();
    int retval = write(sockfd,c_response,smsg.size());
    return retval;
}

std::string SysUtil::get_my_ip(int sockfd){
    struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    std::string str;
    
    if(getsockname(sockfd, (struct sockaddr *) &addr, &size) == 0){
        str = std::string(inet_ntoa(addr.sin_addr));
    }
    
    return str;
    
}

std::string SysUtil::get_error_text(){
    return error_info;
}

void SysUtil::stop_waiting(){
    waiting = false;
}