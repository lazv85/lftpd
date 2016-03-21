#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <iostream>

enum ConfigResponse{READ_OK, ALREADY_READ, FILE_NOT_AVAILABLE, CLEAR_OK, WRONG_LINE, LINE_OK};

class Config{
    private:
        static Config * instance;
        Config();
        std::string config_name;
        std::map<std::string, std::string> key_value;
        ConfigResponse parse_line(std::string line,std::string* key, std::string * val);
        std::string  get_section(std::string line, std::string current_section);
        std::string clear_comment(std::string line);
    public:
        static Config * get_instance();
        std::string get_value(std::string key);
        std::string get_value(std::string section, std::string key);
        ConfigResponse parse_config(std::string file_name);
        ConfigResponse clear();
};

#endif