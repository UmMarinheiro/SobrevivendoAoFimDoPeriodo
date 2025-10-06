#pragma once
#include "colisor.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>
#include <utility>
#include "gameConstants.hpp"
#include "waterDrop.hpp"

class AirConditioner : public Transform
{
private:
    std::shared_ptr<Sprite> sprite_sptr;
    std::vector<std::shared_ptr<WaterDrop>> drops;
    std::chrono::time_point<std::chrono::steady_clock> spawnTime;
    bool coliding;   
    float nextDrop;
public:
    AirConditioner(std::pair<float, float> pos, bool coliding);
    int getTimeFromStart();
    static std::shared_ptr<AirConditioner> createAirConditioner(std::pair<float, float> pos, bool coliding);
    void update();
    void tryMakeDrop();
};