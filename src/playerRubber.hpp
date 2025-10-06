#pragma once
#include "player.hpp"
#include <memory>
#include <string>
#include <vector>
#include "gameInstance.hpp"
#include "rubberBand.hpp"
#include "gameConstants.hpp"

class PlayerRubber : public Player
{
private:
    std::shared_ptr<RubberBand> rubber_sptr;
    GameInstance* gameInstancePtr;
    bool isRubberSpawned = false, hasTargetPos = false;
    std::pair<float,float> targetPos;
    void SpawnRubberBand();
public:
    PlayerRubber(std::string asset, GameInstance* gameInstancePtr);

    void startRec();
    void startPast();
    void updateRec(const std::pair<float, float>* pos, float t);
    void updatePast(float t);
    void endRec();
    void endPast();
};