#include "spriteMan.hpp"
#include <iostream>

SpriteMan* SpriteMan::instance = nullptr;
cv::Mat SpriteMan::windowFrame = cv::Mat();

SpriteMan& SpriteMan::getInstance()
{
    if(!instance) instance = new SpriteMan();
    return *instance;
}


void SpriteMan::prv_addSprite(std::weak_ptr<Sprite> toAdd) {sprites.push_back(toAdd);}
void SpriteMan::prv_tick()
{
    for(auto it = sprites.begin(); it != sprites.end(); it++)
    {
        std::weak_ptr<Sprite>& sprite_wptr = *it;

        if(sprite_wptr.expired()) sprites.erase(it--);
        else sprite_wptr.lock()->draw(windowFrame);
    }
} 

void SpriteMan::addSprite(std::weak_ptr<Sprite> toAdd) {getInstance().prv_addSprite(toAdd);}
void SpriteMan::tick() {getInstance().prv_tick();}