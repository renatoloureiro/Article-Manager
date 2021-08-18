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

#include "spdlog/spdlog.h"
#include "spdlog/async.h" //support for async logging.
#include "spdlog/sinks/basic_file_sink.h"

namespace fs = std::filesystem;

class unit{
    public:
        int x;
        std::string name;
        std::vector <std::string> tag;
};

std::vector <unit> list;
std::vector <std::string> tags;

int main(void){

    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
    auto my_logger = spdlog::basic_logger_mt("basic_logger", "basic.txt");
    my_logger->info("hello");

    spdlog::info("START ARTICLE MANAGER");
    my_logger->info("START ARTICLE MANAGER");

    spdlog::info("check files in folder '/articles'");
    my_logger->info("check files in folder '/articles'");
    

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
        return EXIT_FAILURE;
    }

    std::cout << files[1] << std::endl;
    system("ls articles");
    
    

    unit renato1{5, "mamma mia"};
    unit renato2{6, "mamma poop"};
    list.push_back(renato1);
    list.push_back(renato2);


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