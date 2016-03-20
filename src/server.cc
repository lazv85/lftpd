#include "server.h"

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

#include "cmdfactory.h"
#include "user.h"
#include "pass.h"
#include "port.h"
#include "exception.h"

#define PORT 8080
#define WAIT_TIME 1

bool Server::running = true;
Server * Server::instance = NULL;

Server * Server::get_instance(){
    if(!Server::instance){
        Server::instance = new Server();
    }
    return Server::instance;
}

Server::Server(){
    if(signal(SIGINT, Server::sig_handler)<0){
        Exception e;
        e.text = std::string("Server: signal error: ") + strerror(errno);
        throw e;
    }
}

Server::~Server(){
    close(this->server_socket);
}

int Server::init_server_socket(){
    this->server_socket = socket(AF_INET,SOCK_STREAM,0);
    
    if(this->server_socket < 0){
        Exception e;
        e.text = std::string("Server: socket error: ") + strerror(errno);
        throw e;
    }
    
    int flag = 1 ;
    
    sockaddr_in addr;
    bzero((char *) &addr, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(this->server_socket, (struct sockaddr *) &addr,sizeof(addr)) < 0){
        Exception e;
        e.text = std::string("Server: bind error: ") + strerror(errno);
        throw e;
    }
    
    if(listen(this->server_socket,5) < 0){
        Exception e;
        e.text = std::string("Server: listen error: ") + strerror(errno);
        throw e;
    }
}

void Server::close_server_socket(){
    close(this->server_socket);
}

int Server::wait_on_socket(int socket, int timeout_second){
    fd_set active_fd_set, read_fd_set;
    
    sockaddr_in addr;
    socklen_t size_addr = sizeof(addr);
    struct timeval timeout;
    
    int retval;
    
    do{
        FD_ZERO (&active_fd_set);
        FD_SET (socket, &active_fd_set);
            
        timeout.tv_sec = timeout_second;
        timeout.tv_usec = 0;
        retval = select(socket+1,&active_fd_set,NULL,NULL,&timeout);
            
    }while(retval <= 0 && Server::running);
    
    return retval;
}

int Server::get_client_socket(int server_socket){
    
    sockaddr_in addr;
    socklen_t size_addr = sizeof(addr);
    
    int client_socket = accept(server_socket,(struct sockaddr *) &addr,&size_addr);
    
    if(client_socket<0){
        Exception e;
        e.text = std::string("Server: accept error: ") + strerror(errno);
        throw e;
    }
    
    return client_socket;
}

void Server::say_hello(int socket){
    Server::write_to_socket(socket, std::string("220 Hey there"));
}

void Server::write_to_socket(int socket, std::string msg){
    std::string smsg = msg + "\r\n";
    const char * c_response = smsg.c_str();
    write(socket,c_response,smsg.size());
}

std::string Server::read_from_socket(int socket){
    
    int len;
            
    if(ioctl(socket,FIONREAD,&len)<0){
        Exception e;
        close(socket);
        e.text = std::string("Server: ioctl error: ") + strerror(errno);
        throw e;
    }
            
    char * buffer ;
    buffer = new char[len];
    bzero(buffer,len) ;
    
    if(read(socket,buffer,len)<0){
        Exception e;
        close(socket);
        e.text = std::string("Server: read from socket error: ") + strerror(errno);
        throw e;
    }

    std::string retval = std::string(buffer);
    delete [] buffer;
    
    return retval;
}

Command * Server::need_auth(int socket,Authorize user_password){
    Command * cmd;
    CommandFactory command_factory;
    
    int retval;
    std::string cmd_str;
    std::string what;
    
    if(user_password == AUTH_USER){
        what = "USER";
    }else{
        what = "PASS";
    }
    
    do{
        retval = Server::wait_on_socket(socket,WAIT_TIME);
        cmd_str = Server::read_from_socket(socket);
        cmd = command_factory.get_command(cmd_str);
        
        if(cmd->get_command() != what){
            if(user_password == AUTH_USER){
                Server::write_to_socket(socket,cmd->not_looged_in());
            }
            if(user_password == AUTH_PASSWORD){
                Server::write_to_socket(socket,cmd->need_password());
            }
        }
    }while(cmd->get_command() != what && Server::running);
    
    return cmd;
}

void Server::process_connection(int socket){
    CommandFactory command_factory;
    
    Server:: say_hello(socket);
    
    User * usr ;
    Pass * pwd;
    
    std::string cmd_str;
    int retval;
    
    usr = dynamic_cast <User *> (Server::need_auth(socket,AUTH_USER));
    Server::write_to_socket(socket,usr->get_response());
    
    if(usr->get_status() == PASSWORD_REQUIRED){
        pwd = dynamic_cast <Pass*> (Server::need_auth(socket,AUTH_PASSWORD));
        Server::write_to_socket(socket,pwd->get_response());
    }

    Command* cmd;
    bool quit_connection;
    
    int data_socket = -1;
    
    do{
        retval = Server::wait_on_socket(socket,WAIT_TIME);
        cmd_str = Server::read_from_socket(socket);
        cmd = command_factory.get_command(cmd_str);
        
        if(cmd->get_command() == "PORT"){
            data_socket = dynamic_cast<Port*>(cmd)->get_socket();
        }else if(data_socket != -1 && cmd->is_data()){
            int retval = cmd->transfer_data(data_socket);
            close(data_socket);
            data_socket = -1;
        }
        
        Server::write_to_socket(socket,cmd->get_response());
        quit_connection = cmd->quit_connection();
        delete cmd;
    } while(! quit_connection && Server::running);
    
    delete usr;
}

void Server::loop(){
    init_server_socket();
    int retval ;
    
    int client_socket;
    
    while(Server::running){
        retval = Server::wait_on_socket(this->server_socket,WAIT_TIME);
        
        if(retval>0){
            client_socket = Server::get_client_socket(this->server_socket);
            Server::process_connection(client_socket);
            close(client_socket);
        }
    }
    
    close_server_socket();
}

void Server::sig_handler(int t){
    std::cout << "Got signal: " << t << std::endl;
    Server::running = false;
}