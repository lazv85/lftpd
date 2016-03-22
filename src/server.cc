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
#include <cstdlib>

#include "cmdfactory.h"
#include "user.h"
#include "pass.h"
#include "port.h"
#include "pasv.h"
#include "exception.h"

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
        Exception e(std::string("Server: signal error: ") + strerror(errno));
        throw e;
    }
    p_sysutil = SysUtil::get_instance();
    p_config = Config::get_instance();
    
    if(p_config->parse_config("config.ini")!= CFG_OK){
        Exception e(std::string("configuration problem"));
        throw e;
    }
    check_configuration();
}

Server::~Server(){
    close(this->server_socket);
}

void Server::say_hello(int socket){
    if(p_sysutil->write_to_socket(socket, std::string("220 Hey there"))<0){
        Exception e("Server: say_hello: "+ p_sysutil->get_error_text());
        throw e;
    }
}


Command * Server::need_auth(int socket,std::string what){
    Command * cmd;
    CommandFactory command_factory;
    
    int retval;
    std::string cmd_str;

    do{
        retval = p_sysutil->wait_on_socket(socket,WAIT_TIME);
        
        if(p_sysutil->read_from_socket(socket,&cmd_str)<0){
            Exception e("need_auth:" + p_sysutil->get_error_text());
            throw e;
        }
        cmd = command_factory.get_command(cmd_str);
        
        if(cmd->get_command() != what){
            if(what == "USER"){
                if(p_sysutil->write_to_socket(socket,cmd->not_looged_in())<0){
                    Exception e("need_auth:" + p_sysutil->get_error_text());
                    throw e;
                }
            }
            if(what == "PASS"){
                if(p_sysutil->write_to_socket(socket,cmd->need_password())<0){
                    Exception e("need_auth:" + p_sysutil->get_error_text());
                    throw e;
                }
            }
        }
    }while(cmd->get_command() != what && Server::running);
    
    return cmd;
}

void Server::authorize(int socket){
    
    User * usr ;
    Pass * pwd;
    
    usr = dynamic_cast <User *> (Server::need_auth(socket,"USER"));
    
    if(p_sysutil->write_to_socket(socket,usr->get_response())<0){
        Exception e("process_connection:" + p_sysutil->get_error_text());
        throw e;
    }
    
    if(usr->get_status() == USR_NEED_PASSWORD){
        std::string user_settings = p_config->get_value("users",usr->get_parameter());
        
        pwd = dynamic_cast <Pass*> (Server::need_auth(socket,"PASS"));
        
        while(pwd->authorize(user_settings) == PASS_WRONG_PASSWORD){
            if(p_sysutil->write_to_socket(socket,pwd->get_response())<0){
                Exception e("process_connection:" + p_sysutil->get_error_text());
                throw e;
            }
            delete pwd;
            pwd = dynamic_cast <Pass*> (Server::need_auth(socket,"PASS"));
        }
       
       if(p_sysutil->write_to_socket(socket,pwd->get_response())<0){
                Exception e("process_connection:" + p_sysutil->get_error_text());
                throw e;
        } 
       delete pwd;

    }
    delete usr;
}

void Server::session(int socket){
    CommandFactory command_factory;

    std::string cmd_str;
    int retval;
    
    Command* cmd;
    bool quit_connection;
    
    int data_socket = -1;
    
    do{
        retval = p_sysutil->wait_on_socket(socket,WAIT_TIME);
        if(p_sysutil->read_from_socket(socket,&cmd_str )<0){
            Exception e("process_connection:" + p_sysutil->get_error_text());
            throw e;
        }
        cmd = command_factory.get_command(cmd_str);
        
        if(cmd->get_command() == "PORT"){
            if(data_socket != -1){
                close(data_socket);
                data_socket = -1;
            }
            data_socket = dynamic_cast<Port*>(cmd)->get_socket();
        } else if(cmd->get_command() == "PASV"){
            int pasv_sockfd = dynamic_cast<Pasv*>(cmd)->get_socket(p_sysutil->get_my_ip(socket));
            
            if(p_sysutil->write_to_socket(socket,cmd->get_response())<0){
                Exception e("process_connection:" + p_sysutil->get_error_text());
                throw e;
            }
            
            data_socket = dynamic_cast<Pasv*>(cmd)->get_data_socket(pasv_sockfd);
        }else if(data_socket != -1 && cmd->is_data()){
            if(p_sysutil->write_to_socket(socket,cmd->transfer_status())<0){
                Exception e("process_connection:" + p_sysutil->get_error_text());
                throw e;
            }
            retval = cmd->transfer_data(data_socket);
            close(data_socket);
            data_socket = -1;
        }
        
        if(cmd->get_command() != "PASV"){
            if(p_sysutil->write_to_socket(socket,cmd->get_response())<0){
                Exception e("process_connection:" + p_sysutil->get_error_text());
            }
        }
        quit_connection = cmd->quit_connection();
        delete cmd;
    } while(! quit_connection && Server::running);
}

void Server::process_connection(int socket){
    
    Server:: say_hello(socket);
    
    Server::authorize(socket);
    
    Server::session(socket);
}

void Server::loop(){
    std::string port = p_config->get_value("system","port");
    if(port.empty()){
        Exception e("loop: no port defined to listen on! Adjust configuration file and restart!");
        throw e;
    }
    
    this->server_socket = p_sysutil->init_server_socket(atoi(port.c_str()));
    if(this->server_socket<0){
        Exception e("loop:" + p_sysutil->get_error_text());
        throw e;
    }
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

void Server::check_configuration(){
    std::string str ;
    
    str = p_config->get_value("system","port");
    if(str.empty()){
        Exception e("configuration: system.port has to be defined");
        throw e;
    }
    
    str = p_config->get_value("system","anonymous_access");
    if(str.empty() || (str != "yes" && str != "no")){
        Exception e("configuration: system.anonymous_access has to be defined. values:yes, no");
        throw e;
    }
}

void Server::sig_handler(int t){
    std::cout << "Got signal: " << t << std::endl;
    Server::running = false;
    SysUtil * p_sysutil = SysUtil::get_instance();
    p_sysutil->stop_waiting();
}