#pragma once
#include "colisor.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>
#include <utility>

#define SPLASH_IMAGE_SIZE {50, 50}
#define SPLASH_HITBOX_SIZE {50, 50}
#define SPLASH_IMAGE "assets/orange.png"

class InkSplash : public Transform
{
private:
    std::shared_ptr<Sprite> sprite_sptr;
    std::shared_ptr<Colisor> colisor_sptr;
    
public:
    InkSplash(std::pair<float, float> pos, bool coliding);
    static std::shared_ptr<InkSplash> createInkSplash(std::pair<float, float> pos, bool coliding);
};