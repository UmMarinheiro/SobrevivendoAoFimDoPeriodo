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
    float nextDrop;
public:
    PlayerBag(std::string asset);

    void tryMakeDrop(float t, bool coliding);

    void startRec();
    void startPast();
    void updateRec(const std::pair<float, float>* pos, float t);
    void updatePast(float t);
    void endRec();
    void endPast();
};