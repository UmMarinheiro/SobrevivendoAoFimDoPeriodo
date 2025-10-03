#pragma once
#include "player.hpp"
#include <memory>
#include <string>
#include <vector>
#include "inkSplash.hpp"
#include "gameConstants.hpp"

class PlayerPen : public Player
{
private:
    std::vector<std::shared_ptr<InkSplash>> splashes;
    float nextSplash;
public:
    PlayerPen(std::string asset);

    void tryMakeSplash(float t, bool coliding);

    void startRec();
    void startPast();
    void updateRec(const std::pair<float, float>* pos, float t);
    void updatePast(float t);
    void endRec();
    void endPast();
};