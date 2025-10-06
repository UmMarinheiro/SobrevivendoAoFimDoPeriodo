#include "airConditioner.hpp"
#include "gameConstants.hpp"
#include "waterDrop.hpp"
#include <memory>
#include <iostream>
#include <utility>

AirConditioner::AirConditioner(std::pair<float, float> pos, bool coliding) : coliding(coliding)
{    
    setGlobalPos(pos);
    sprite_sptr = Sprite::createSprite(PLAYERAIR_DEVICE_IMAGE, (Transform*)this);
    sprite_sptr->setLocalSize(PLAYERAIR_DEVICE_SIZE);
    nextDrop = PLAYERAIR_DROP_INTERVAL_MS;
    
    spawnTime = std::chrono::steady_clock::now();
}
std::shared_ptr<AirConditioner> AirConditioner::createAirConditioner(std::pair<float, float> pos, bool coliding)
{
    return std::make_shared<AirConditioner>(pos, coliding);
}
int AirConditioner::getTimeFromStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - spawnTime).count();
}
void AirConditioner::update()
{
    tryMakeDrop();
    for (auto &drop : drops) drop->update();
}
void AirConditioner::tryMakeDrop()
{
    if(getTimeFromStart() > nextDrop)
    {
        drops.push_back(WaterDrop::createWaterDrop(getGlobalPos(), coliding));
        nextDrop += PLAYERPEN_SPLASH_INTERVAL_MS;
    }
}