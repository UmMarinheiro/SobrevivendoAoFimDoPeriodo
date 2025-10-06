#include "playerAir.hpp"
#include "player.hpp"
#include "transform.hpp"
#include <iostream>

PlayerAir::PlayerAir(std::string asset) : Player(asset){}


void PlayerAir::startRec() 
{
    Player::startRec();
    air_sptr = AirConditioner::createAirConditioner(record[0].pos, false);
}
void PlayerAir::startPast() 
{
    Player::startPast();
    air_sptr = AirConditioner::createAirConditioner(record[0].pos, true);
}
void PlayerAir::updateRec(const std::pair<float, float>* pos, float t)
{
    Player::updateRec(pos, t);
    air_sptr->update();
}
void PlayerAir::updatePast(float t)
{
    Player::updatePast(t);
    air_sptr->update();
}
void PlayerAir::endRec()
{
    air_sptr.reset();
}
void PlayerAir::endPast()
{
    air_sptr.reset();
}