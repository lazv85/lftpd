#include "list.h"
#include <unistd.h>
List::List(std::string cmd, std::string param) : Command(cmd,param) {
    
}

std::string List::get_response(){
    std::string str;
    switch(status){
        case TRANSMITTED:
            str = std::string("226 Directory or File information successfully transmitted.");
            break;
        case NOCONNECTION:
            str = std::string("425 No TCP connection to transfer data.");
            break;
        case NETWORKFAILURE:
            str = std::string("426 TCP connection has been broken.");
            break;
        case CANNOTREADDISK:
            str = std::string("451 File or Directory is not available.");
            break;
    }
    return str;
}

bool List::is_data(){
    return true;
}

int List::transfer_data(int socket){
    std::string smsg = std::string("hello") + "\r\n";
    const char * c_response = smsg.c_str();
    write(socket,c_response,smsg.size());
    status = TRANSMITTED;
    return 0;
}