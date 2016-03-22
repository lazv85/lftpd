#include "config.h"
#include <boost/regex.hpp>
#include <iostream>
#include <fstream>

Config * Config::instance = NULL;

Config::Config(){
    
}

Config * Config::get_instance(){
    if(!instance){
        instance = new Config();
    }
    return instance;
}

CFG_CODES Config::parse_config(std::string file_name){
    
    std::string line;
    
    if(this->config_name.empty() ){
        this->config_name = file_name;
    }else{
        return CFG_ALREADY_READ;
    }
    
    std::ifstream f(file_name.c_str(), std::ifstream::in);

    if(!f.is_open() ){
        return CFG_FILE_NOT_AVAILABLE;
    }
    
    std::string section("default") ;
    std::string key;
    std::string value;
    
    while ( getline (f,line) ){
      line = clear_comment(line);
      section = get_section(line, section);
      section = trim(section);
      
      if(parse_line(line,&key, &value) == CFG_OK){
          key_value[section + trim(key)] = trim(value);
      }
      
    }
    
    f.close();
    
    return CFG_OK;
}

CFG_CODES Config::clear(){
    key_value.clear();
    config_name = "";
    return CFG_OK;
}

CFG_CODES Config::parse_line(std::string line, std::string * key, std::string * value){
    boost::regex rgx("([^=]+)\\s*=\\s*(.+)");
    boost::smatch match;
      
    if (boost::regex_search(line, match, rgx)){
        *key = match[1];
        *value = match[2];
    }else{
        return CFG_WRONG_LINE;
    }
    
    return CFG_OK;
}

std::string Config::get_section(std::string line, std::string current_section){
    boost::regex rgx("\\[(\\w+)\\]");
    boost::smatch match;

    if (boost::regex_search(line, match, rgx)){
        std::string str = match[1];
        return str;
    }else{
        return current_section;
    }
}

std::string Config::clear_comment(std::string line){
    boost::regex rgx("(^[^#]+)#?");
    boost::smatch match;
    
    std::string str;
    
    if(boost::regex_search(line, match, rgx)){
        str =  std::string(match[1]);
    }
    
    return str;
}


std::string Config::get_value(std::string key){
    return get_value("default", key);
}


std::string Config::get_value(std::string section, std::string key){
   std::map<std::string,std::string>::iterator it = key_value.find(section + key);
   std::string str;
   if(it != key_value.end()){
       str = it->second;
   }
   return str;
}

std::string Config::get_config_name(){
    return config_name;
}

std::string Config::trim(std::string str){
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));

}