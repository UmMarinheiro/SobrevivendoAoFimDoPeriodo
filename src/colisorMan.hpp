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
    std::vector<std::string> prv_getColisions(const Colisor* colisor);
    std::vector<std::string> prv_getColisionsStartingWith(const Colisor* colisor, std::string filter); 
    std::vector<std::string> prv_getColisionsWithPoint(std::pair<float,float> point);
public:
    static void addColisor(std::weak_ptr<Colisor> toAdd);
    static std::vector<std::string> getColisions(const Colisor* colisor);
    static std::vector<std::string> getColisionsStartingWith(const Colisor* colisor, std::string filter);
    static std::vector<std::string> getColisionsWithPoint(std::pair<float,float> point);
};