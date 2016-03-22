#include <unistd.h>
#include <boost/regex.hpp>
#include "pasv.h"
#include "sysutil.h"
#include <string>
#include <sstream>

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

Pasv::Pasv(std::string cmd, std::string param) : Command(cmd, param) {
    
}

std::string Pasv::get_response(){

    std::string port_str1;
    std::string port_str2;
    
    port_str1 = to_string(port1);
    port_str2 = to_string(port2);
    
    std::string str = ip + ","+port_str1+","+port_str2;
    
    str = "227 Entering Passive Mode (" + str + ")";
    return str;
}

int Pasv::get_socket(std::string ip_address){
    
    SysUtil * p_sysutil = SysUtil::get_instance();
    ip = boost::regex_replace(ip_address, boost::regex("\\."), std::string(","));
    int sockfd;
    
    do{
        port1 = std::rand()%100 + 4;
        port2 = std::rand()%100 + 4;
        sockfd = p_sysutil->init_server_socket(256*port1 + port2);
    }while(sockfd <0);
    
    return sockfd;
}

int Pasv::get_data_socket(int sockfd){
    SysUtil * p_sysutil = SysUtil::get_instance();
    
    int cl_sockfd = p_sysutil->get_client_socket(sockfd,5);
    close(sockfd);
    return cl_sockfd;
}