#include <boost/regex.hpp>
#include <iostream>
#include "cmdfactory.h"
#include "user.h"
#include "abor.h"
#include "cdup.h"
#include "cwd.h"
#include "dele.h"
#include "epsv.h"
#include "help.h"
#include "list.h"
#include "mdtm.h"
#include "mkd.h"
#include "nlst.h"
#include "noop.h"
#include "pass.h"
#include "pasv.h"
#include "port.h"
#include "pwd.h"
#include "quit.h"
#include "rein.h"
#include "retr.h"
#include "rmd.h"
#include "size.h"
#include "stor.h"
#include "syst.h"
#include "type.h"

int CommandFactory::set_command_details(std::string cmd){
    boost::regex rgx("([a-zA-Z]+)(\\s+([a-zA-Z0-9,]+))?");
    boost::smatch match;
    
    if (boost::regex_search(cmd, match, rgx)){
        this->command = match[1];
        this->parameters = match[3];
        std::cout <<"command=" << this->command << std::endl;
        std::cout <<"parameters=" << this->parameters << std::endl;
        return 0;
    }
    
    return -1;
}

Command* CommandFactory::get_command(std::string cmd){
    
    if(this->set_command_details(cmd)<0){
        return new Command(cmd,NULL);
    }
    
    if (this->command == "USER")
            return dynamic_cast<Command *> (new User(this->command, this->parameters));
    if (this->command == "ABOR")
            return dynamic_cast<Command *> (new Abor(this->command, this->parameters));
    if (this->command ==  "CDUP")
            return dynamic_cast<Command *> (new Cdup(this->command, this->parameters));
    if (this->command ==  "CWD")
            return dynamic_cast<Command *> (new Cwd(this->command, this->parameters));
    if (this->command ==  "DELE")
            return dynamic_cast<Command *> (new Dele(this->command, this->parameters));
    if (this->command ==  "EPSV")
            return dynamic_cast<Command *> (new Epsv(this->command, this->parameters));
    if (this->command ==  "HELP")
            return dynamic_cast<Command *> (new Help(this->command, this->parameters));
    if (this->command ==  "LIST")
            return dynamic_cast<Command *> (new List(this->command, this->parameters));
    if (this->command ==  "MDTM")
            return dynamic_cast<Command *> (new Mdtm(this->command, this->parameters));
    if (this->command ==  "MKD")
            return dynamic_cast<Command *> (new Mkd(this->command, this->parameters));
    if (this->command ==  "NLST")
            return dynamic_cast<Command *> (new Nlst(this->command, this->parameters));
    if (this->command ==  "NOOP")
            return dynamic_cast<Command *> (new Noop(this->command, this->parameters));
    if (this->command ==  "PASS")
            return dynamic_cast<Command *> (new Pass(this->command, this->parameters));
    if (this->command ==  "PASV")
            return dynamic_cast<Command *> (new Pasv(this->command, this->parameters));
    if (this->command ==  "PORT")
            return dynamic_cast<Command *> (new Port(this->command, this->parameters));
    if (this->command ==  "PWD")
            return dynamic_cast<Command *> (new Pwd(this->command, this->parameters));
    if (this->command ==  "QUIT")
            return dynamic_cast<Command *> (new Quit(this->command, this->parameters));
    if (this->command ==  "REIN")
            return dynamic_cast<Command *> (new Rein(this->command, this->parameters));
    if (this->command ==  "RETR")
            return dynamic_cast<Command *> (new Retr(this->command, this->parameters));
    if (this->command ==  "RMD")
            return dynamic_cast<Command *> (new Rmd(this->command, this->parameters));
    if (this->command ==  "SIZE")
            return dynamic_cast<Command *> (new Size(this->command, this->parameters));
    if (this->command ==  "STOR")
            return dynamic_cast<Command *> (new Stor(this->command, this->parameters));
    if (this->command ==  "SYST")
            return dynamic_cast<Command *> (new Syst(this->command, this->parameters));
    if (this->command ==  "TYPE")
            return dynamic_cast<Command *> (new Type(this->command, this->parameters));
    
    return new Command(this->command,this->parameters);

}