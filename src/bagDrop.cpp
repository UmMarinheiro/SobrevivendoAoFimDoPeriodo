#include "bagDrop.hpp"
#include "gameConstants.hpp"
#include <memory>
#include <iostream>
#include <utility>

BagDrop::BagDrop(std::pair<float, float> pos, bool coliding) : coliding(coliding)
{    
    setGlobalPos(pos);
    sprite_sptr = Sprite::createSprite(PLAYERBAG_DROP_IMAGE, (Transform*)this);
    sprite_sptr->setLocalSize(PLAYERBAG_DROP_START_SIZE);
    fullyGrown = false;
    
    spawnTime = std::chrono::steady_clock::now();
    lastFrameTime = 0;
}
std::shared_ptr<BagDrop> BagDrop::createBagDrop(std::pair<float, float> pos, bool coliding)
{
    return std::make_shared<BagDrop>(pos, coliding);
}
int BagDrop::getTimeFromStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - spawnTime).count();
}
void BagDrop::update()
{
    int currentTime = getTimeFromStart();
    {
        float deltaTime = (float)(currentTime - lastFrameTime)/1000;
        
        auto [x, y] = getGlobalPos();
        setGlobalPos({x,y+PLAYERBAG_DROP_VELOCITY*deltaTime});
        
        lastFrameTime = currentTime;
    }
    
    if(fullyGrown) return;

    float growthPercentage = (float)currentTime/PLAYERBAG_DROP_GROWTH_MS;
    sprite_sptr->setLocalSize({
        (int[2])PLAYERBAG_DROP_START_SIZE[0]+((int[2])PLAYERBAG_DROP_IMAGE_SIZE[0] - (int[2])PLAYERBAG_DROP_START_SIZE[0])*growthPercentage,
        (int[2])PLAYERBAG_DROP_START_SIZE[1]+((int[2])PLAYERBAG_DROP_IMAGE_SIZE[1] - (int[2])PLAYERBAG_DROP_START_SIZE[1])*growthPercentage
    });

    if(growthPercentage < 1) return;
    
    fullyGrown = true;
    sprite_sptr->setLocalSize(PLAYERBAG_DROP_IMAGE_SIZE);

    if(!coliding) return;
    colisor_sptr = Colisor::createColisor("damaging",(Transform*)this);
    colisor_sptr->setLocalSize(PLAYERBAG_DROP_HITBOX_SIZE);     
}