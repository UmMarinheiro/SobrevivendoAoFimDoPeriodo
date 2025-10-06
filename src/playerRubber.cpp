#include "playerRubber.hpp"
#include "player.hpp"
#include <iostream>

PlayerRubber::PlayerRubber(std::string asset, GameInstance* gameInstancePtr) : Player(asset)
{
    this->gameInstancePtr = gameInstancePtr;
}

void PlayerRubber::SpawnRubberBand()
{
    auto pos = getGlobalPos();
    if(!hasTargetPos) 
    {
        targetPos = gameInstancePtr->getCurrentPlayer()->getGlobalPos();
        gameInstancePtr = nullptr;
        hasTargetPos = true;
    }
    rubber_sptr = RubberBand::createRubberBand(pos, targetPos);
}
void PlayerRubber::startRec() 
{
    Player::startRec();
}
void PlayerRubber::startPast() 
{
    Player::startPast();
    isRubberSpawned = false;
}
void PlayerRubber::updateRec(const std::pair<float, float>* pos, float t)
{
    Player::updateRec(pos, t);
}
void PlayerRubber::updatePast(float t)
{
    Player::updatePast(t);
    if(!isRubberSpawned && t > PLAYERRUBBER_SPAWN_TIME)
    {
        SpawnRubberBand();
        isRubberSpawned = true;
    }
    if(rubber_sptr != nullptr) rubber_sptr->update();
}
void PlayerRubber::endRec()
{
    rubber_sptr.reset();
    Player::endRec();
}
void PlayerRubber::endPast()
{
    rubber_sptr.reset();
    Player::endPast();
}