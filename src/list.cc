#include "list.h"
#include <unistd.h>
#include <dirent.h>
#include <algorithm>

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

std::vector<std::string> List::list_directory(std::string dir_name){
    std::vector<std::string> dir_content;
    DIR *dr;
    std::string smsg;
    
    struct dirent *fl;

    dr = opendir(dir_name.c_str());
    
    if(dr == NULL){
        status = CANNOTREADDISK;
    }
    
    do {
        fl=readdir(dr);
        if(fl != NULL){
            smsg = std::string(fl->d_name) + "\r\n";
            dir_content.push_back(smsg);
        }
    }while(fl != NULL && status != CANNOTREADDISK);
    
    closedir(dr);
    
    status = TRANSMITTED;    
    return dir_content;
};

int List::transfer_data(int socket){
    int bytes = 0;
    
    std::vector<std::string> dir_content;
    dir_content = list_directory(".");
    
    if(status == CANNOTREADDISK){
        return bytes;
    }
    
    std::sort(dir_content.begin(),dir_content.end());
    
    for(std::vector<std::string>::iterator it = dir_content.begin(); it != dir_content.end() && status != NETWORKFAILURE; ++it) {
        int retval = write(socket,it->c_str(),it->size());
        if(retval < 0){
            status = NETWORKFAILURE;
            bytes = 0;
        }else{
            bytes += retval;
        }
    }
    
    status = TRANSMITTED;
    return bytes;
}

std::string List::transfer_status(){
    std::string str("150 Here comes info");
    return str;
}