#pragma once
#include <utility>
#include "colisor.hpp"
#include "sprite.hpp"
#include <memory>
#include <vector>

#define BUFFERSIZE 8
#define PLAYER_IMAGE_SIZE {50,50}
#define PLAYER_HITBOX_SIZE {50,50}

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
    std::shared_ptr<Colisor> colisor_sptr;
    std::pair<float,float> buffer[BUFFERSIZE];
    int current = 0;
    bool filled = false;
    bool alive = true;

public:
    Player(std::string asset);
    static std::shared_ptr<Player> createPlayer(std::string asset, int item);

    void appendPosToBuffer(const std::pair<float, float> &pos);
    std::pair<float, float> getPosFromBuffer();
    std::pair<float, float> getPosFromRecordings(float t);
    bool isAlive();

    virtual void startRec();
    virtual void startPast();
    virtual void updateRec(const std::pair<float, float>* pos, float t);
    virtual void updatePast(float t);
    virtual void endRec();
    virtual void endPast();
};