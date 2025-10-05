#pragma once
#include "player.hpp"
#include <memory>
#include <string>
#include <vector>
#include "gameConstants.hpp"
#include "beggingDog.hpp"

class PlayerDog : public Player
{
private:
    std::shared_ptr<BeggingDog> dog_sptr;
public:
    PlayerDog(std::string asset);

    void startRec();
    void startPast();
    void updateRec(const std::pair<float, float>* pos, float t);
    void updatePast(float t);
    void endRec();
    void endPast();
};