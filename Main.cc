#include <iostream>
#include "src/server.h"
#include "src/config.h"
#include "src/exception.h"

int main() {
    Server * srv;
    Config * cfg;
    
    try{
        srv = Server::get_instance();
        cfg = Config::get_instance();
        std::cout << "FTP Server started at port " << cfg->get_value("system","port") <<"!" << std::endl;
        srv->loop();
        delete srv;
        
    }catch(Exception e){
        if(!srv)
            delete srv;
        std::cout<< "got some error: " << e.text << std::endl;
    }
    
   return 0; 
}
