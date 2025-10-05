#include "inkSplash.hpp"
#include "gameConstants.hpp"
#include <memory>
#include <iostream>
#include <utility>

InkSplash::InkSplash(std::pair<float, float> pos, bool coliding) : coliding(coliding)
{    
    setGlobalPos(pos);
    sprite_sptr = Sprite::createSprite(PLAYERPEN_SPLASH_IMAGE, (Transform*)this);
    sprite_sptr->setLocalSize(PLAYERPEN_SPLASH_START_SIZE);
    fullyGrown = false;
    
    spawnTime = std::chrono::steady_clock::now();
}
std::shared_ptr<InkSplash> InkSplash::createInkSplash(std::pair<float, float> pos, bool coliding)
{
    return std::make_shared<InkSplash>(pos, coliding);
}
int InkSplash::getTimeFromStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - spawnTime).count();
}
void InkSplash::update()
{
    if(fullyGrown) return;

    float growthPercentage = (float)getTimeFromStart()/PLAYERPEN_SPLASH_GROWTH_MS;
    sprite_sptr->setLocalSize({
        (int[2])PLAYERPEN_SPLASH_START_SIZE[0]+((int[2])PLAYERPEN_SPLASH_IMAGE_SIZE[0] - (int[2])PLAYERPEN_SPLASH_START_SIZE[0])*growthPercentage,
        (int[2])PLAYERPEN_SPLASH_START_SIZE[1]+((int[2])PLAYERPEN_SPLASH_IMAGE_SIZE[1] - (int[2])PLAYERPEN_SPLASH_START_SIZE[1])*growthPercentage
    });

    if(growthPercentage < 1) return;
    
    fullyGrown = true;
    sprite_sptr->setLocalSize(PLAYERPEN_SPLASH_IMAGE_SIZE);

    if(!coliding) return;
    colisor_sptr = Colisor::createColisor("damaging",(Transform*)this);
    colisor_sptr->setLocalSize(PLAYERPEN_SPLASH_HITBOX_SIZE);     
}