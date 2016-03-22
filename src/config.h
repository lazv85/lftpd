#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <iostream>
#include <string>

enum CFG_CODES{CFG_OK, CFG_ALREADY_READ, CFG_FILE_NOT_AVAILABLE, CFG_WRONG_LINE};

class Config{
    private:
        static Config * instance;
        Config();
        std::string config_name;
        std::map<std::string, std::string> key_value;
        CFG_CODES parse_line(std::string line,std::string* key, std::string * val);
        std::string  get_section(std::string line, std::string current_section);
        std::string clear_comment(std::string line);
        std::string trim(std::string);
    public:
        static Config * get_instance();
        std::string get_value(std::string key);
        std::string get_value(std::string section, std::string key);
        CFG_CODES parse_config(std::string file_name);
        CFG_CODES clear();
        std::string get_config_name();
};

#endif