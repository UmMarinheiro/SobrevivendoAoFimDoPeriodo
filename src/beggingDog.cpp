#include "beggingDog.hpp"
#include "gameConstants.hpp"
#include "transform.hpp"
#include <cmath>
#include <memory>
#include <iostream>
#include <utility>

BeggingDog::BeggingDog(std::pair<float, float> pos, Transform* player, bool coliding) : coliding(coliding), player(player)
{    
    setGlobalPos(pos);
    sprite_sptr = Sprite::createSprite(PLAYERDOG_IMAGE, (Transform*)this);
    sprite_sptr->setLocalSize(PLAYERDOG_START_SIZE);
    fullyGrown = false;
    
    spawnTime = std::chrono::steady_clock::now();
    lastFrameTime = 0;
}
std::shared_ptr<BeggingDog> BeggingDog::createDog(std::pair<float, float> pos, Transform* player, bool coliding)
{
    return std::make_shared<BeggingDog>(pos, player, coliding);
}
int BeggingDog::getTimeFromStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - spawnTime).count();
}
std::pair<float, float> BeggingDog::getPlayerDir()
{
    auto [x, y] = getGlobalPos();
    auto [playerX, playerY] = player->getGlobalPos();
    std::pair<float, float> dir = {playerX - x, playerY - y};
    float magnitude = std::sqrt(dir.first*dir.first + dir.second*dir.second);
    dir.first /= magnitude;
    dir.second /= magnitude;
    return dir;
}
void BeggingDog::update()
{
    int currentTime = getTimeFromStart();
    
    if(fullyGrown) 
    {
        float deltaTime = (float)(currentTime - lastFrameTime)/1000;
        
        auto [x, y] = getGlobalPos();
        auto [vx, vy] = getPlayerDir();
        vx *= PLAYERDOG_VELOCITY; 
        vy *= PLAYERDOG_VELOCITY;
        setGlobalPos({x + vx*deltaTime, y + vy*deltaTime});
        lastFrameTime = currentTime;
    }
    else
    {
        lastFrameTime = currentTime;
        float growthPercentage = (float)currentTime/PLAYERDOG_GROWTH_MS;
        sprite_sptr->setLocalSize({
            (int[2])PLAYERDOG_START_SIZE[0]+((int[2])PLAYERDOG_IMAGE_SIZE[0] - (int[2])PLAYERDOG_START_SIZE[0])*growthPercentage,
            (int[2])PLAYERDOG_START_SIZE[1]+((int[2])PLAYERDOG_IMAGE_SIZE[1] - (int[2])PLAYERDOG_START_SIZE[1])*growthPercentage
        });

        if(growthPercentage < 1) return;

        fullyGrown = true;
        sprite_sptr->setLocalSize(PLAYERDOG_IMAGE_SIZE);

        if(!coliding) return;
        colisor_sptr = Colisor::createColisor("damaging",(Transform*)this);
        colisor_sptr->setLocalSize(PLAYERDOG_HITBOX_SIZE); 
    }    
}