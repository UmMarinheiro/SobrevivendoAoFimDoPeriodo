#include "inkSplash.hpp"
#include <memory>
#include <iostream>
#include <utility>

InkSplash::InkSplash(std::pair<float, float> pos, bool coliding)
{    
    setGlobalPos(pos);
    sprite_sptr = Sprite::createSprite(SPLASH_IMAGE, (Transform*)this);
    sprite_sptr->setLocalSize(SPLASH_IMAGE_SIZE);
    
    if(!coliding) return;
    colisor_sptr = Colisor::createColisor("damaging",(Transform*)this);
    colisor_sptr->setLocalSize(SPLASH_HITBOX_SIZE);    
}
std::shared_ptr<InkSplash> InkSplash::createInkSplash(std::pair<float, float> pos, bool coliding)
{
    return std::make_shared<InkSplash>(pos, coliding);
}