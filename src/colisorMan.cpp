#include "colisorMan.hpp"
#include "colisor.hpp"
#include <string>
#include <vector>

ColisorMan* ColisorMan::instance = nullptr;

ColisorMan& ColisorMan::getInstance()
{
    if(!instance) instance = new ColisorMan();
    return *instance;
}


void ColisorMan::prv_addColisor(std::weak_ptr<Colisor> toAdd) {colisors.push_back(toAdd);}
std::vector<std::string> ColisorMan::prv_getColisions(const Colisor* colisor){return prv_getColisionsStartingWith(colisor,"");}
std::vector<std::string> ColisorMan::prv_getColisionsStartingWith(const Colisor* colisor, std::string filter)
{
    std::vector<std::string> colided;
    for(auto it = colisors.begin(); it != colisors.end(); it++)
    {
        std::weak_ptr<Colisor>& current_wptr = *it;

        if(current_wptr.expired()) colisors.erase(it--);
        else 
        {
            std::shared_ptr<Colisor> current = current_wptr.lock();
            if(colisor->getIdentifier() != current->getIdentifier() &&
                colisor->getIdentifier().compare(current->getIdentifier()) != 0 &&
                current->isCollidingWith(colisor)) 
            {
                colided.push_back(current->getIdentifier());
            }
        } 
    }
    return colided;
} 
std::vector<std::string> ColisorMan::prv_getColisionsWithPoint(std::pair<float,float> point)
{
    std::vector<std::string> colided;
    for(auto it = colisors.begin(); it != colisors.end(); it++)
    {
        std::weak_ptr<Colisor>& current_wptr = *it;

        if(current_wptr.expired()) colisors.erase(it--);
        else 
        {
            std::shared_ptr<Colisor> current = current_wptr.lock();
            if(current->isPointInside(point)) 
            {
                colided.push_back(current->getIdentifier());
            }
        } 
    }
    return colided;
}

void ColisorMan::addColisor(std::weak_ptr<Colisor> toAdd) {getInstance().prv_addColisor(toAdd);}
std::vector<std::string> ColisorMan::getColisions(const Colisor* colisor) {getInstance().prv_getColisions(colisor);}
std::vector<std::string> ColisorMan::getColisionsStartingWith(const Colisor* colisor, std::string filter) {getInstance().prv_getColisionsStartingWith(colisor,filter);}
std::vector<std::string> ColisorMan::getColisionsWithPoint(std::pair<float, float> point){getInstance().getColisionsWithPoint(point);}