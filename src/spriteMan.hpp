#pragma once
#include <memory>
#include <list>
#include "sprite.hpp"

class SpriteMan
{
private:
    static SpriteMan* instance;

    std::list<std::weak_ptr<Sprite>> sprites;
    
    SpriteMan() {}
    ~SpriteMan() {}
    static SpriteMan& getInstance();
    
    void prv_updateList();
    void prv_addSprite(std::weak_ptr<Sprite> toAdd);
    void prv_tick(); 
public:
    static cv::Mat windowFrame;
    
    static void updateList();
    static void addSprite(std::weak_ptr<Sprite> toAdd);
    static void tick(); 
};