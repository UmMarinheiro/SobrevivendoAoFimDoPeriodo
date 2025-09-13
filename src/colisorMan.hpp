#pragma once
#include <memory>
#include <list>
#include <string>
#include <vector>
#include "colisor.hpp"

class ColisorMan
{
private:
    static ColisorMan* instance;

    std::list<std::weak_ptr<Colisor>> colisors;
    
    ColisorMan() {}
    ~ColisorMan() {}
    static ColisorMan& getInstance();
    
    void prv_addColisor(std::weak_ptr<Colisor> toAdd);
    std::vector<std::string> prv_getColisions(Colisor& colisor);
    std::vector<std::string> prv_getColisionsStartingWith(Colisor& colisor, std::string filter); 
public:
    static void addColisor(std::weak_ptr<Colisor> toAdd);
    static std::vector<std::string> getColisions(Colisor& colisor);
    static std::vector<std::string> getColisionsStartingWith(Colisor& colisor, std::string filter);
};