#include "list.h"
#include <unistd.h>
#include <dirent.h>
#include <algorithm>

List::List(std::string cmd, std::string param) : Command(cmd,param) {
    
}

std::string List::get_response(){
    std::string str;
    switch(status){
        case LST_OK:
            str = std::string("226 Directory or File information successfully transmitted.");
            break;
        case LST_NO_CONN:
            str = std::string("425 No TCP connection to transfer data.");
            break;
        case LST_NETWORK_ERR:
            str = std::string("426 TCP connection has been broken.");
            break;
        case LST_READ_ERR:
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
        status = LST_READ_ERR;
    }
    
    do {
        fl=readdir(dr);
        if(fl != NULL){
            smsg = std::string(fl->d_name) + "\r\n";
            dir_content.push_back(smsg);
        }
    }while(fl != NULL && status != LST_READ_ERR);
    
    closedir(dr);
    
    status = LST_OK;    
    return dir_content;
};

int List::transfer_data(int socket){
    int bytes = 0;
    
    std::vector<std::string> dir_content;
    dir_content = list_directory(".");
    
    if(status == LST_READ_ERR){
        return bytes;
    }
    
    std::sort(dir_content.begin(),dir_content.end());
    
    for(std::vector<std::string>::iterator it = dir_content.begin(); it != dir_content.end() && status != LST_NETWORK_ERR; ++it) {
        int retval = write(socket,it->c_str(),it->size());
        if(retval < 0){
            status = LST_NETWORK_ERR;
            bytes = 0;
        }else{
            bytes += retval;
        }
    }
    
    status = LST_OK;
    return bytes;
}

std::string List::transfer_status(){
    std::string str("150 Here comes info");
    return str;
}