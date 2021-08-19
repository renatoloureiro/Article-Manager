#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <dirent.h>
#include <yaml-cpp/yaml.h>

#include "spdlog/spdlog.h"
#include "spdlog/async.h" //support for async logging.
#include "spdlog/sinks/basic_file_sink.h"

#include "class.hpp"

std::vector <unit> upload(std::vector <unit> list, std::shared_ptr<spdlog::logger> my_logger){
    std::fstream newfile;
    newfile.open(strcat(get_current_dir_name() , "/bin/data.txt"),std::ios::in);
    if(newfile.is_open()) {
        std::string tp;
        unit aux{};
        while(getline(newfile, tp)){ //read data from file object and put it into string.
            
            char space_char = ' ';
            std::vector<std::string> words;

            std::stringstream sstream(tp);
            std::string word;
            while (std::getline(sstream, word, space_char)){
                //word.erase(std::remove_if(word.begin(), word.end(), isspace), word.end());
                words.push_back(word);
            }
            if (words[0]=="name"){
                aux.name=words[1];
            }
            if (words[0]=="number"){
                aux.number=std::stoi(words[1]);
            }
            if (words[0]=="tag"){
                for(int i=1; i<words.size();i++){
                    (aux.tag).push_back(words[i]);
                }
            }
            if (words[0]=="--break"){
                list.push_back(aux);
                aux=aux.clear(aux);
            }
            
            
        }
        newfile.close(); 
    } else {
        spdlog::error("open data.txt");
        my_logger->error("open data.txt");
        newfile.close(); 
        exit(-1);
    }

    return list;
}

std::vector<char *> read_files_from_directory(std::shared_ptr<spdlog::logger> my_logger){
    DIR *dir; struct dirent *diread;
    std::vector<char *> files;
    
    if ((dir = opendir("/home/renato/Documents/git-repo/Article-Manager/articles")) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {
            if(strcmp( diread->d_name, ".") && strcmp( diread->d_name, "..")){
                files.push_back(diread->d_name);
            }
        }
        closedir (dir);
    } else {
        spdlog::error("opendir /articles");
        my_logger->error("opendir /articles");
        exit(-1);
    }
    return files;
}

config_state config_upload(){

    if(std::filesystem::exists("config.yaml")==0){
        spdlog::error("config.yaml does not exist in current directory");
        spdlog::error("manager will exit");
        exit(-1);
    };

    config_state manager_config;

    YAML::Node config = YAML::LoadFile("config.yaml");

    /*if (config["lastLogin"]) {
        std::cout << "Last logged in: " << config["lastLogin"].as<std::DateTime>() << "\n";
    }*/

    manager_config.username = config["username"].as<std::string>();
    manager_config.password = config["password"].as<std::string>();
    //manager_config.directory = config["directory"].as<std::string>();

    //login(username, password);
    //config["lastLogin"] = std::getCurrentDateTime();

    //std::ofstream fout("config.yaml");
    //fout << config;
    return manager_config;

}

int save_list(std::vector <unit> list){

    for(int i=0; i<list.size();i++){
        std::cout<< "name " << list[i].name << std::endl;
        std::cout<< "number " << list[i].number << std::endl;
    }
    return 0;
}

std::ostream& bold_on(std::ostream& os){
    return os << "\e[1m";
}
std::ostream& bold_off(std::ostream& os){
    return os << "\e[0m";
}


#endif