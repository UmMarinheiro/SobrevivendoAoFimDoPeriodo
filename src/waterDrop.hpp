#pragma once
#include "colisor.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>
#include <utility>
#include "gameConstants.hpp"

class WaterDrop : public Transform
{
private:
    std::shared_ptr<Sprite> sprite_sptr;
    std::shared_ptr<Colisor> colisor_sptr;
    std::chrono::time_point<std::chrono::steady_clock> spawnTime;
    int lastFrameTime;
    bool fullyGrown; 
    bool coliding;   
public:
    WaterDrop(std::pair<float, float> pos, bool coliding);
    int getTimeFromStart();
    int getDeltaTime();
    static std::shared_ptr<WaterDrop> createWaterDrop(std::pair<float, float> pos, bool coliding);
    void update();
};