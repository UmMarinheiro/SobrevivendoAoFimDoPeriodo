#pragma once
#include <utility>
#include "colisor.hpp"
#include "sprite.hpp"
#include <memory>
#include <vector>
#include "gameConstants.hpp"

std::pair<float, float> getRectCenter(cv::Rect r);

typedef struct TimeStamp
{
    std::pair<float, float> pos;
    float t;
}TimeStamp;

class Player : public Transform
{
private:
    std::vector<TimeStamp> record = {(TimeStamp){{0,0}, 0}};

    std::shared_ptr<Sprite> sprite_sptr;
    std::shared_ptr<Colisor> colisor_sptr;
    std::pair<float,float> buffer[BUFFERSIZE];
    int current = 0;
    bool filled = false;
    bool alive = true;

    std::pair<float, float> handleRawRecPos(const std::pair<float, float>* posPtr);

public:
    Player(std::string asset);
    static std::shared_ptr<Player> createPlayer(std::string asset, int item);

    void appendPosToBuffer(const std::pair<float, float> &pos);
    std::pair<float, float> getPosFromBuffer();
    std::pair<float, float> getPosFromRecordings(float t);
    bool isAlive();

    virtual void startPositioningRec();
    virtual void startPositioningPast();
    virtual void updatePositioningRec(const std::pair<float, float>* posPtr);
    virtual void updatePositioningPast();
    virtual void startRec();
    virtual void startPast();
    virtual void updateRec(const std::pair<float, float>* posPtr, float t);
    virtual void updatePast(float t);
    virtual void endRec();
    virtual void endPast();
};