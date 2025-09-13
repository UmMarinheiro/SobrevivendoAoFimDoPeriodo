#pragma once
#include "transform.hpp"
#include <memory>
#include <string>
#include <vector>

class Colisor : public Transform
{
private:
    std::string identifier;
    Colisor();
public:
    static std::shared_ptr<Colisor> createColisor();

    std::string getIdentifier();
    void setIdentifier(std::string identifier);
    std::vector<std::string> getColisions();
    std::vector<std::string> getColisionsStartingWith(std::string filter);
};