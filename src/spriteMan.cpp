#include "spriteMan.hpp"
#include <algorithm>
#include <iostream>
#include <utility>

SpriteMan* SpriteMan::instance = nullptr;
cv::Mat SpriteMan::windowFrame = cv::Mat();

SpriteMan& SpriteMan::getInstance()
{
    if(!instance) instance = new SpriteMan();
    return *instance;
}

void SpriteMan::prv_updateList()
{
    for(auto it = sprites.begin(); it != sprites.end(); it++)
    {
        std::weak_ptr<Sprite>& sprite_wptr = *it;
        if(sprite_wptr.expired()) sprites.erase(it--);
    }
    for(int i = 1; i < sprites.size; i++) 
    {
        for(auto it = sprites.begin()+1; it != sprites.end(); it++)
        {
            Sprite& sprite_current = **it;
            Sprite& sprite_previous = **(it-1);
            if(sprite_current.getLayer() < sprite_previous.getLayer()) std::swap(*it,*(it-1));
        }
    }
}
void SpriteMan::prv_addSprite(std::weak_ptr<Sprite> toAdd) 
{
    sprites.push_back(toAdd);
    updateList();
}
void SpriteMan::prv_tick()
{
    for(auto it = sprites.begin(); it != sprites.end(); it++)
    {
        std::weak_ptr<Sprite>& sprite_wptr = *it;

        if(sprite_wptr.expired()) sprites.erase(it--);
        else sprite_wptr.lock()->draw(windowFrame);
    }
} 

void SpriteMan::updateList() {getInstance().prv_updateList();}
void SpriteMan::addSprite(std::weak_ptr<Sprite> toAdd) {getInstance().prv_addSprite(toAdd);}
void SpriteMan::tick() {getInstance().prv_tick();}