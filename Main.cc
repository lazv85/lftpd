#include <iostream>
#include "src/server.h"
#include "src/exception.h"

int main() {
    Server * srv;
    try{
        std::cout << "FTP Server started!" << std::endl;
        srv = Server::get_instance();
        srv->loop();
        delete srv;
        
    }catch(Exception e){
        if(!srv)
            delete srv;
        std::cout<< "got some error: " << e.text << std::endl;
    }
    
   return 0; 
}
