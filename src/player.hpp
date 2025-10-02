#pragma once
#include <utility>
#include "sprite.hpp"
#include <memory>
#include <vector>

#define BUFFERSIZE 8

std::pair<float, float> getRectCenter(cv::Rect r);

typedef struct TimeStamps
{
    std::pair<float, float> pos;
    float t;
}TimeStamps;

class Player : public Transform
{
private:
    std::vector<TimeStamps> record;

    std::shared_ptr<Sprite> sprite_sptr;
    std::pair<float,float> buffer[BUFFERSIZE];
    int current = 0;
    bool filled = false;

public:
    Player(std::string asset);
    static std::shared_ptr<Player> createPlayer(std::string asset, int item);

    void startRec();
    void startPast();
    void updateRec(std::pair<float, float> pos, float t);
    void updatePast(float t);
    void endRec();
    void endPast();
};