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
#include "pasv.h"
#include "exception.h"

#define SERVER_PORT 8080
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
    p_sysutil = SysUtil::get_instance();
}

Server::~Server(){
    close(this->server_socket);
}

void Server::say_hello(int socket){
    p_sysutil->write_to_socket(socket, std::string("220 Hey there"));
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
        retval = p_sysutil->wait_on_socket(socket,WAIT_TIME);
        cmd_str = p_sysutil->read_from_socket(socket);
        cmd = command_factory.get_command(cmd_str);
        
        if(cmd->get_command() != what){
            if(user_password == AUTH_USER){
                p_sysutil->write_to_socket(socket,cmd->not_looged_in());
            }
            if(user_password == AUTH_PASSWORD){
                p_sysutil->write_to_socket(socket,cmd->need_password());
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
    p_sysutil->write_to_socket(socket,usr->get_response());
    
    if(usr->get_status() == PASSWORD_REQUIRED){
        pwd = dynamic_cast <Pass*> (Server::need_auth(socket,AUTH_PASSWORD));
        p_sysutil->write_to_socket(socket,pwd->get_response());
    }

    Command* cmd;
    bool quit_connection;
    
    int data_socket = -1;
    
    do{
        retval = p_sysutil->wait_on_socket(socket,WAIT_TIME);
        cmd_str = p_sysutil->read_from_socket(socket);
        cmd = command_factory.get_command(cmd_str);
        
        if(cmd->get_command() == "PORT"){
            if(data_socket != -1){
                close(data_socket);
                data_socket = -1;
            }
            data_socket = dynamic_cast<Port*>(cmd)->get_socket();
        } else if(cmd->get_command() == "PASV"){
            if(data_socket != -1){
                close(data_socket);
                data_socket = -1;
            }
            p_sysutil->write_to_socket(socket,cmd->get_response());
            data_socket = dynamic_cast<Pasv*>(cmd)->get_socket();
        }else if(data_socket != -1 && cmd->is_data()){
            p_sysutil->write_to_socket(socket,cmd->transfer_status());
            retval = cmd->transfer_data(data_socket);
            close(data_socket);
            data_socket = -1;
        }
        
        p_sysutil->write_to_socket(socket,cmd->get_response());
        quit_connection = cmd->quit_connection();
        delete cmd;
    } while(! quit_connection && Server::running);
    
    delete usr;
}

void Server::loop(){
    this->server_socket = p_sysutil->init_server_socket(SERVER_PORT);
    int retval ;
    
    int client_socket;
    
    while(Server::running){
        retval = p_sysutil->wait_on_socket(this->server_socket,WAIT_TIME);
        
        if(retval>0){
            client_socket = p_sysutil->get_client_socket(this->server_socket, 0);
            Server::process_connection(client_socket);
            close(client_socket);
        }
    }
    
    close(this->server_socket);
}

void Server::sig_handler(int t){
    std::cout << "Got signal: " << t << std::endl;
    Server::running = false;
    SysUtil * p_sysutil = SysUtil::get_instance();
    p_sysutil->stop_waiting();
}