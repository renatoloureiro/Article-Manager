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

namespace fs = std::filesystem;

int main(void){

    fs::path path = fs::current_path() / "articles";
    std::cout << path << std::endl;
    for (const auto & file : fs::directory_iterator(path)){
        std::cout << file.path() << std::endl;
    }

    return EXIT_SUCCESS;
}