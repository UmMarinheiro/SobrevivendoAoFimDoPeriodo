#pragma once
#include "player.hpp"
#include <memory>
#include <string>
#include <vector>
#include "bagDrop.hpp"
#include "gameConstants.hpp"

class PlayerBag : public Player
{
private:
    std::vector<std::shared_ptr<BagDrop>> drops;
    float nextSplash;
public:
    PlayerBag(std::string asset);

    void tryMakeSplash(float t, bool coliding);

    void startRec();
    void startPast();
    void updateRec(const std::pair<float, float>* pos, float t);
    void updatePast(float t);
    void endRec();
    void endPast();
};