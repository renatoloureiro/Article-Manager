/*
Author: Renato Loureiro
email: renato.loureiro@tecnico.ulisboa.pt
*/

#include <iostream>
#include <unistd.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <yaml-cpp/yaml.h>

#include "spdlog/spdlog.h"
#include "spdlog/async.h" 
#include "spdlog/sinks/basic_file_sink.h"

#include "class.hpp"
#include "functions.hpp"

namespace fs = std::filesystem;


//define global variables
std::vector <std::string> tags;


int main(void){

    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
    std::shared_ptr<spdlog::logger> my_logger = spdlog::basic_logger_mt("basic_logger", "basic.txt");

    spdlog::info("START ARTICLE MANAGER");
    my_logger->info("START ARTICLE MANAGER");

    //upload config parameters from config.yaml
    config_state manager_config=config_upload();

    std::vector <unit> list;

    spdlog::info("check files in folder '/articles'");
    my_logger->info("check files in folder '/articles'");
    
    // read files from directory where articles are stored
    std::vector<char *> files=read_files_from_directory(my_logger);
    
    // upload list of current data inside .txt
    list=upload(list, my_logger);

    save_list(list);

    while(1){
        std::string mystrg;
        std::cout <<">> ";
        getline(std::cin, mystrg);

        if(mystrg == "show"){
            system("ls articles");
        }

        if(mystrg == "--help"){

            std::fstream newfile;
            newfile.open("help.txt",std::ios::in);
            if(newfile.is_open()) {

                std::string tp;
                while(getline(newfile, tp)){ //read data from file object and put it into string.
                    std::cout << tp << "\n"; //print the data of the string
                }
                newfile.close(); 
            } else {
                spdlog::error("open help.txt");
                my_logger->error("open help.txt");
                newfile.close(); 
                return EXIT_FAILURE;
            }
        }

        if(mystrg == "quit"){
            spdlog::info("end session");
            my_logger->info("end session");
            return EXIT_FAILURE;
        }

    }

    return EXIT_SUCCESS;
}