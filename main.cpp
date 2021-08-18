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
        std::string name;
        int number;
        std::vector <std::string> tag;
        unit clear(unit aux);
};

unit unit::clear(unit aux){
    aux.tag.clear();
    aux.name.clear();
    aux.number=-1;
    return aux;
}


std::vector <std::string> tags;

int main(void){

    std::vector <unit> list;

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
    
    // upload list of current data inside .txt
    std::fstream newfile;
    newfile.open("data.txt",std::ios::in);
    if(newfile.is_open()) {
        std::string tp;
        unit aux{};
        while(getline(newfile, tp)){ //read data from file object and put it into string.
            
            char space_char = ' ';
            std::vector<std::string> words{};

            std::stringstream sstream(tp);
            std::string word;
            while (std::getline(sstream, word, space_char)){
                word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
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
            if (words[0]=="break"){
                list.push_back(aux);
                aux=aux.clear(aux);
            }
            
            
        }
        newfile.close(); 
        } else {
            spdlog::error("open help.txt");
            my_logger->error("open help.txt");
            newfile.close(); 
            return EXIT_FAILURE;
        }

    //

    unit renato1{"mamma mia",5 };
    unit renato2{"mamma mia", 6};
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