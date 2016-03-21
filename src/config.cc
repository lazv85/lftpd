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

ConfigResponse Config::parse_config(std::string file_name){
    
    std::string line;
    
    if(!this->config_name){
        this->config_name = file_name;
    }else{
        return ALREADY_READ;
    }
    
   std::ifstream f(file_name);
    
    if(!f.is_open() ){
        return FILE_NOT_AVAILABLE;
    }
    
    std::string section("default") ;
    std::string key;
    std::string value;
    
    while ( getline (f,line) ){
      line = clear_comment(line);
      section = get_section(line, section);
      
      if(parse_line(line,&key, &value) == LINE_OK){
          key_value[section + key] = value;
      }
      
    }
    
    f.close();
    
    return READ_OK;
}

ConfigResponse Config::clear(){
    config_data.clear();
    return CLEAR_OK;
}

ConfigResponse Config::parse_line(std::string line, std::string * key, std::string * value){
    boost::regex rgx("([^#]=[])#?");
    boost::smatch match;
    
    return LINE_OK;
}

std::string Config::get_section(std::string line, std::string current_section){
    boost::regex rgx("\\[([^\\]+)\\]\\]");
    boost::smatch match;
    
    if (boost::regex_search(line, match, rgx)){
        std::string str = match[1];
        return str;
    }else{
        return current_section;
    }
}

std::string Config::clear_comment(std::string line){
    boost::regex rgx("([^#]+)#?");
    boost::smatch match;
    
    std::string str;
    
    if(boost::regex_search(line, match, rgx)){
        str =  std::string(match[1]);
    }
    
    return str;
}


std::string get_value(std::string key){
    return get_value("default", key);
}


std:string get_value(std::string section, std::string key){
    return key_value[section + key];
}