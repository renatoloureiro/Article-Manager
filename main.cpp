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
#include <sys/types.h>

#if __has_include("spdlog/spdlog.h") && __has_include("spdlog/async.h") && __has_include("spdlog/sinks/basic_file_sink.h")
int spdlog_stat=1;
#include "spdlog/spdlog.h"
#include "spdlog/async.h" 
#include "spdlog/sinks/basic_file_sink.h"
std::shared_ptr<spdlog::logger> my_logger;
#else 
#warning "[WARNING] spdlog not instaled"
int spdlog_stat =0;
#endif

//#include "spdlog/spdlog.h"
//#include "spdlog/async.h" 
//#include "spdlog/sinks/basic_file_sink.h"

#include "src/class.hpp"
#include "src/functions.hpp"

namespace fs = std::filesystem;


//define global variables
std::vector <std::string> tags;


int main(void){
    
    if(spdlog_stat==0){
        std::cout << bold_on <<"[WARNING]" << bold_off<<" spdlog not instaled" <<std::endl;
        std::cout << bold_on <<"[ERROR]" << bold_off << " manager will exit" << std::endl;
        exit(-1);
    }

    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
    my_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");

    spdlog::info("START ARTICLE MANAGER");
    my_logger->info("START ARTICLE MANAGER");

    //upload config parameters from config.yaml
    config_state manager_config=config_upload();

    char *pass = getpass("\nEnter Administrator Password: ");
    if(strcmp(pass,"loureiro1") !=0){
        spdlog::error("wrong password");
        spdlog::error("manager will exit");
        exit(-1);
    }
    
    std::vector <unit> list;

    spdlog::info("check files in folder '/articles'");
    my_logger->info("check files in folder '/articles'");


    // read files from directory where articles are stored
    std::vector<char *> files=read_files_from_directory(my_logger);
    

    // upload list of current data inside .txt
    list=upload(list, my_logger);


    /*struct stat fileInfo;
    if (stat(strcat(get_current_dir_name() , "/articles/Arob_2teste_89708.pdf"), &fileInfo) != 0) {  // Use stat() to get the info
      std::cerr << "Error: " << strerror(errno) << '\n';
      return(EXIT_FAILURE);
   }

    std::cout << "Size          : " << fileInfo.st_size << '\n';               // Size in bytes
    std::cout << "Device        : " << (char)(fileInfo.st_dev + 'A') << '\n';  // Device number
    std::cout << "Created       : " << std::ctime(&fileInfo.st_ctime);         // Creation time
    std::cout << "Modified      : " << std::ctime(&fileInfo.st_mtime);         // Last mod time
    */


    //save_list(list);

    while(1){
        std::string mystrg;
        std::cout <<">> ";
        getline(std::cin, mystrg);

        char space_char = ' ';
        std::stringstream sstream(mystrg);
        std::vector<std::string> words;
        std::string word;
        while (std::getline(sstream, word, space_char)){
            //word.erase(std::remove_if(word.begin(), word.end(), isspace), word.end());
            words.push_back(word);
        }


        if(words[0] =="add"){
            if (words.size()==1){
                std::cout << bold_on << "error " << bold_off << "in add, too few arguments" << std::endl;
            }else{

                if (words[1]=="-n"){
                    // read number that I have for each file still to be uploaded
                    std::vector<char *> files=read_files_from_directory(my_logger);
                    unit aux{};
                    aux.name=files[stoi(words[2])];
                    (aux.tag).push_back(words[4]);
                    //list.emplace_back(aux);
                    list.push_back(aux);

                }else{
                    std::vector<char *> files=read_files_from_directory(my_logger);
                    int a=0;
                    for (int i=0; i<files.size(); i++){
                        if(words[1]==files[i]){ //verify if file exists
                            a=1;
                        }
                    }
                    if (a==1){
                        std::cout << "the file exists and it will be added"<< std::endl;
                        unit aux{};
                        aux.name=words[1];
                        (aux.tag).push_back(words[3]);
                        list.push_back(aux);
                    }else{
                        std::cout << "the file does NOT exist" << std::endl;
                    }
                }
            }            
        }


        if(words[0] == "show"){

            //check the size of words first
            //if(words[1]=="-f"){
            //    system("ls articles");
            //}
            
            for (int i=0; i<list.size() ;i++){
                std::cout <<  bold_on << "["<<list[i].number <<  "]"<< bold_off <<  "\t" <<list[i].name  << "\t" << bold_on <<"--tag" << bold_off <<"\t";
                for (int j = 0; j < list[i].tag.size(); j++){
                    std::cout << list[i].tag[j] <<"\t";
                }
                std::cout << "\n";
            }

        }


        if(words[0] == "--help"){

            std::fstream newfile;
            newfile.open("bin/help.txt",std::ios::in);
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

        if(words[0]=="status"){

            std::vector<char *> files=read_files_from_directory(my_logger);
            std::cout << bold_on <<"["<< files.size() <<"]"<< bold_off <<" files in current directory" <<std::endl;
            std::cout << bold_on <<"["<< list.size() <<"]"<< bold_off <<" files uploaded" <<std::endl;
            std::cout << "files to upload:" << std::endl;
            
            int aux=0;
            for(int i=0; i<files.size(); i++){
                int k=0;
                for (int j = 0; j < list.size(); j++){
                    if (list[j].name==files[i]){
                        k++;
                    }
                }
                if(k==0){
                    std::cout << "    " <<bold_on << "["<<aux<<"] "<< bold_off << files[i] << std::endl;
                    aux++;
                }
            }
            //std::cout << bold_on <<"["<< aux <<"]"<< bold_off <<" files NOT uploaded" <<std::endl;
        }



        if(words[0] == "quit"){
            // save data in .txt
            // rewrite list data 

            save_list(list);

            spdlog::info("end session");
            my_logger->info("end session");
            return EXIT_FAILURE;
            //exit(-1);
        }

    }

    return EXIT_SUCCESS;
}