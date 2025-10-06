#include "rubberBand.hpp"
#include "gameConstants.hpp"
#include <memory>
#include <iostream>
#include <utility>
#include <cmath>

void RubberBand::setVelocity(std::pair<float, float> pos, std::pair<float, float> target)
{
    auto [x, y] = pos;
    auto [targetX, targetY] = target;
    velocity = {targetX - x, targetY - y};
    float magnitude = std::sqrt(velocity.first*velocity.first + velocity.second*velocity.second);
    velocity.first /= magnitude;
    velocity.second /= magnitude;

    velocity.first *= PLAYERRUBBER_VELOCITY;
    velocity.second *= PLAYERRUBBER_VELOCITY;
}
RubberBand::RubberBand(std::pair<float, float> pos, std::pair<float, float> target)
{    
    setGlobalPos(pos);
    sprite_sptr = Sprite::createSprite(PLAYERRUBBER_IMAGE, (Transform*)this);
    sprite_sptr->setLocalSize(PLAYERRUBBER_START_SIZE);
    fullyGrown = false;
    setVelocity(pos, target);

    spawnTime = std::chrono::steady_clock::now();
    lastFrameTime = 0;
}
std::shared_ptr<RubberBand> RubberBand::createRubberBand(std::pair<float, float> pos, std::pair<float, float> target)
{
    return std::make_shared<RubberBand>(pos, target);
}
int RubberBand::getTimeFromStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - spawnTime).count();
}
void RubberBand::update()
{
    int currentTime = getTimeFromStart();
    
    
    if(fullyGrown)
    {
        float deltaTime = (float)(currentTime - lastFrameTime)/1000;
        
        auto [x, y] = getGlobalPos();
        auto [vx, vy] = velocity;
        setGlobalPos({x + vx*deltaTime, y + vy*deltaTime});
    }
    else 
    {
        float growthPercentage = (float)getTimeFromStart()/PLAYERRUBBER_GROWTH_MS;
        sprite_sptr->setLocalSize({
            (int[2])PLAYERRUBBER_START_SIZE[0]+((int[2])PLAYERRUBBER_IMAGE_SIZE[0] - (int[2])PLAYERRUBBER_START_SIZE[0])*growthPercentage,
            (int[2])PLAYERRUBBER_START_SIZE[1]+((int[2])PLAYERRUBBER_IMAGE_SIZE[1] - (int[2])PLAYERRUBBER_START_SIZE[1])*growthPercentage
        });

        if(growthPercentage < 1) return;
        
        fullyGrown = true;
        sprite_sptr->setLocalSize(PLAYERRUBBER_IMAGE_SIZE);
        colisor_sptr = Colisor::createColisor("damaging",(Transform*)this);
        colisor_sptr->setLocalSize(PLAYERRUBBER_HITBOX_SIZE);     
    }
    lastFrameTime = currentTime;
}