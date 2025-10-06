#pragma once
#include "colisor.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>
#include <utility>
#include "gameConstants.hpp"

class RubberBand : public Transform
{
private:
    std::shared_ptr<Sprite> sprite_sptr;
    std::shared_ptr<Colisor> colisor_sptr;
    std::chrono::time_point<std::chrono::steady_clock> spawnTime;
    bool fullyGrown; 
    std::pair<float, float> velocity;
    int lastFrameTime;
    void setVelocity(std::pair<float, float> pos, std::pair<float, float> target);
public:
    RubberBand(std::pair<float, float> pos, std::pair<float, float> target);
    int getTimeFromStart();
    static std::shared_ptr<RubberBand> createRubberBand(std::pair<float, float> pos, std::pair<float, float> target);
    void update();
};