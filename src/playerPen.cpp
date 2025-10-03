#include "playerPen.hpp"
#include "inkSplash.hpp"
#include "player.hpp"
#include <iostream>

PlayerPen::PlayerPen(std::string asset) : Player(asset){}

void PlayerPen::tryMakeSplash(float t, bool coliding)
{
    if(t > nextSplash)
    {
        splashes.push_back(InkSplash::createInkSplash(getGlobalPos(), coliding));
        nextSplash += SPLASH_INTERVAL_MS;
    }
}

void PlayerPen::startRec() 
{
    nextSplash = SPLASH_INTERVAL_MS;
    Player::startRec();
}
void PlayerPen::startPast() 
{
    nextSplash = SPLASH_INTERVAL_MS;
    Player::startPast();
}
void PlayerPen::updateRec(const std::pair<float, float>* pos, float t)
{
    Player::updateRec(pos, t);
    tryMakeSplash(t, false);
}
void PlayerPen::updatePast(float t)
{
    Player::updatePast(t);
    tryMakeSplash(t, true);
}
void PlayerPen::endRec()
{
    splashes.clear();
    Player::endRec();
}
void PlayerPen::endPast()
{
    splashes.clear();
    Player::endPast();
}