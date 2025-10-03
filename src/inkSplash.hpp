#pragma once
#include "colisor.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>
#include <utility>
#include "gameConstants.hpp"

class InkSplash : public Transform
{
private:
    std::shared_ptr<Sprite> sprite_sptr;
    std::shared_ptr<Colisor> colisor_sptr;
    std::chrono::time_point<std::chrono::steady_clock> spawnTime;
    bool fullyGrown; 
    bool coliding;   
public:
    InkSplash(std::pair<float, float> pos, bool coliding);
    int getTimeFromStart();
    static std::shared_ptr<InkSplash> createInkSplash(std::pair<float, float> pos, bool coliding);
    void update();
};