#ifndef _SERVER_H_
#define _SERVER_H_ 1
#include <sys/time.h>
#include <string>
#include "command.h"
#include "sysutil.h"


enum Authorize{AUTH_USER,AUTH_PASSWORD};

class Server{
    private:
        int server_socket;
        static bool running;
        static void sig_handler(int);
        Server();
        static Server * instance;
        
        SysUtil * p_sysutil;
        void say_hello(int socket);
        Command* need_auth(int socket, Authorize user_password);
        void process_connection(int socket);
        
    public:
        static Server * get_instance();
         ~Server();
       
        void loop();
        
};

#endif
