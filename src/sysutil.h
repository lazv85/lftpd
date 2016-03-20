#ifndef SYSUTIL_H
#define SYSUTIL_H

#include <string>

class SysUtil{
    private:
        static SysUtil * instance;
        SysUtil();
        bool waiting;
    public:
        ~SysUtil();
        static SysUtil * get_instance();
        int init_server_socket(int portnum);
        int wait_on_socket(int sockfd, int timeout_second);
        int get_client_socket(int sockfd, int timeout_second);
        std::string read_from_socket(int sockfd);
        int write_to_socket(int sockfd, std::string msg);
        void stop_waiting();
};

#endif