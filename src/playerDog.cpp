#include "playerDog.hpp"
#include "player.hpp"
#include "transform.hpp"
#include <iostream>

PlayerDog::PlayerDog(std::string asset) : Player(asset){}


void PlayerDog::startRec() 
{
    Player::startRec();
    dog_sptr = BeggingDog::createDog(record[0].pos, (Transform*)this, false);
}
void PlayerDog::startPast() 
{
    Player::startPast();
    dog_sptr = BeggingDog::createDog(record[0].pos, (Transform*)this, true);
}
void PlayerDog::updateRec(const std::pair<float, float>* pos, float t)
{
    Player::updateRec(pos, t);
    dog_sptr->update();
}
void PlayerDog::updatePast(float t)
{
    Player::updatePast(t);
    dog_sptr->update();
}
void PlayerDog::endRec()
{
    dog_sptr.reset();
}
void PlayerDog::endPast()
{
    dog_sptr.reset();
}