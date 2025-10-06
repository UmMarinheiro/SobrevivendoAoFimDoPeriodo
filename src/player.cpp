#include "player.hpp"
#include "colisor.hpp"
#include <memory>
#include <string>
#include <utility>
#include "playerPen.hpp"
#include "playerBag.hpp"
#include "playerDog.hpp"
#include "playerRubber.hpp"

std::pair<float, float> getRectCenter(cv::Rect r) {return {r.x + r.width/2, r.y + r.height/2};}

Player::Player(std::string asset)
{
    sprite_sptr = Sprite::createSprite(asset, (Transform*)this);
    sprite_sptr->setLocalSize(PLAYER_IMAGE_SIZE);
    colisor_sptr = Colisor::createColisor("Player", (Transform*)this);
    colisor_sptr->setLocalSize(PLAYER_HITBOX_SIZE);
}

void Player::appendPosToBuffer(const std::pair<float, float> &pos)
{
    buffer[current] = pos;

    current++;
    if(current>=8)filled=true;
    current%=BUFFERSIZE;
}
std::pair<float, float> Player::getPosFromBuffer()
{
    std::pair<float, float> val = {0,0};
    char bufferCount = filled?BUFFERSIZE:current;
    for(int i = 0; i < bufferCount; i++)
    {
        val.first += buffer[i].first;
        val.second += buffer[i].second;
    }
    val.first /= bufferCount;
    val.second /= bufferCount;

    return val;
}
std::pair<float, float> Player::getPosFromRecordings(float t)
{
    int i = 0;
    for(i = 0; i < record.size()-1; i++) if(record[i+1].t > t) break;
    return record[i].pos;
}
std::pair<float, float> Player::handleRawRecPos(const std::pair<float, float>* posPtr)
{
    if(posPtr != nullptr) appendPosToBuffer(*posPtr);
    std::pair<float, float> pos = getPosFromBuffer();
    setGlobalPos(pos);
    return pos;
}
bool Player::isAlive(){return alive;}


void Player::startPositioningRec()
{

}
void Player::startPositioningPast()
{
    setGlobalPos(record[0].pos);
}
void Player::updatePositioningRec(const std::pair<float, float>* posPtr)
{
    std::pair<float, float> pos = handleRawRecPos(posPtr);
    record[0] = {pos, 0};
}
void Player::updatePositioningPast() 
{
    
}
void Player::startRec()
{

}
void Player::startPast()
{

}
void Player::updateRec(const std::pair<float, float>* posPtr, float t)
{
    std::pair<float, float> pos = handleRawRecPos(posPtr);
    record.push_back({pos,t});
    if(colisor_sptr->getColisionsStartingWith("damaging").size() > 0) alive = false;
}
void Player::updatePast(float t)
{
    setGlobalPos(getPosFromRecordings(t));
}
void Player::endRec()
{

}
void Player::endPast()
{

}