#include "playerBag.hpp"
#include "bagDrop.hpp"
#include "player.hpp"
#include <iostream>

PlayerBag::PlayerBag(std::string asset) : Player(asset){}

void PlayerBag::tryMakeDrop(float t, bool coliding)
{
    if(t > nextDrop)
    {
        drops.push_back(BagDrop::createBagDrop(getGlobalPos(), coliding));
        nextDrop += PLAYERBAG_DROP_INTERVAL_MS;
    }
}

void PlayerBag::startRec() 
{
    nextDrop = PLAYERBAG_DROP_INTERVAL_MS;
    Player::startRec();
}
void PlayerBag::startPast() 
{
    nextDrop = PLAYERBAG_DROP_INTERVAL_MS;
    Player::startPast();
}
void PlayerBag::updateRec(const std::pair<float, float>* pos, float t)
{
    Player::updateRec(pos, t);
    tryMakeDrop(t, false);
    for(auto& splash_sptr : drops) splash_sptr -> update();
}
void PlayerBag::updatePast(float t)
{
    Player::updatePast(t);
    tryMakeDrop(t, true);
    for(auto& splash_sptr : drops) splash_sptr -> update();
}
void PlayerBag::endRec()
{
    drops.clear();
    Player::endRec();
}
void PlayerBag::endPast()
{
    drops.clear();
    Player::endPast();
}