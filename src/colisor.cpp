#include "colisor.hpp"
#include "colisorMan.hpp"
Colisor::Colisor(std::string identifier) : identifier(identifier){}

std::shared_ptr<Colisor> Colisor::createColisor(std::string identifier)
{
    auto colisor_sptr = std::make_shared<Colisor>(identifier);
    ColisorMan::addColisor(colisor_sptr);
    return colisor_sptr;
}

std::string Colisor::getIdentifier() const{return identifier;}
void Colisor::setIdentifier(std::string identifier) {this->identifier = identifier;}
std::vector<std::string> Colisor::getColisions()
{
    return ColisorMan::getColisions(this);
}
std::vector<std::string> Colisor::getColisionsStartingWith(std::string filter) 
{
    return ColisorMan::getColisionsStartingWith(this,filter);
}
bool Colisor::isPointInside(std::pair<float,float> point)
{
    auto [x,y] = getGlobalPos();
    auto [w,h] = getGlobalSize();
    return x-w/2 < point.first && point.first < x+w/2 &&
           y-h/2 < point.second && point.second < y+h/2;
}
bool Colisor::isCollidingWith(const Colisor* other) const
{
    return ((this->getRect())&(other->getRect())).area > MIN_PIXEL_FOR_COLISION;
}
cv::Rect Colisor::getRect() const
{

    auto [x,y] = getGlobalPos();
    auto [w,h] = getGlobalSize();
    return {x,y,w,h};
}