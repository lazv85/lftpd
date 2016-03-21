#ifndef SYSUTIL_H
#define SYSUTIL_H

#include <string>

class SysUtil{
    private:
        static SysUtil * instance;
        SysUtil();
        bool waiting;
        std::string error_info;
    public:
        ~SysUtil();
        static SysUtil * get_instance();
        int init_server_socket(int portnum);
        int wait_on_socket(int sockfd, int timeout_second);
        int get_client_socket(int sockfd, int timeout_second);
        int read_from_socket(int sockfd, std::string * str);
        int write_to_socket(int sockfd, std::string msg);
        void stop_waiting();
        std::string get_error_text();
};

#endif