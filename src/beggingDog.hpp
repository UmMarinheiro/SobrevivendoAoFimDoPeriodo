#pragma once
#include "colisor.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>
#include <utility>
#include "gameConstants.hpp"

class BeggingDog : public Transform
{
private:
    std::shared_ptr<Sprite> sprite_sptr;
    std::shared_ptr<Colisor> colisor_sptr;
    std::chrono::time_point<std::chrono::steady_clock> spawnTime;
    Transform* player;
    int lastFrameTime;
    bool fullyGrown; 
    bool coliding;   
public:
    BeggingDog(std::pair<float, float> pos, Transform* player, bool coliding);
    std::pair<float, float> getPlayerDir();
    int getTimeFromStart();
    static std::shared_ptr<BeggingDog> createDog(std::pair<float, float> pos, Transform* player, bool coliding);
    void update();
};