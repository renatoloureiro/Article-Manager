#ifndef CLASS_HPP
#define CLASS_HPP

#include <string>
#include <vector>

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

class config_state{
    public:
        std::string username;
        std::string password;
};

#endif