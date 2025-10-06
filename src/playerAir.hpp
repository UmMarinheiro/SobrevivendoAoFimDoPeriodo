#pragma once
#include "player.hpp"
#include <memory>
#include <string>
#include <vector>
#include "gameConstants.hpp"
#include "airConditioner.hpp"

class PlayerAir : public Player
{
private:
    std::shared_ptr<AirConditioner> air_sptr;
public:
    PlayerAir(std::string asset);

    void startRec();
    void startPast();
    void updateRec(const std::pair<float, float>* pos, float t);
    void updatePast(float t);
    void endRec();
    void endPast();
};