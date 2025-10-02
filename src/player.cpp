#include "player.hpp"
#include <memory>
#include <string>
#include <utility>

std::pair<float, float> getRectCenter(cv::Rect r) {return {r.x + r.width/2, r.y + r.height/2};}

Player::Player(std::string asset)
{
    sprite_sptr = Sprite::createSprite(asset, (Transform*)this);
    sprite_sptr->setLocalSize({1,1});
    setLocalSize({50,50});
}
std::shared_ptr<Player> Player::createPlayer(std::string asset, int item)
{
    if(item == 1) return std::make_shared<Player>("assets/arrascaeta.jpeg");//std::make_shared<PlayerPen>(asset);
    // item pode ser substituido por um enum
    // else if(item == tal) std::make_shared<tal>(asset)
    else return std::make_shared<Player>(asset);
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

void Player::startRec()
{

}
void Player::startPast()
{

}
void Player::updateRec(std::pair<float, float> pos, float t)
{
    appendPosToBuffer(pos);
    pos = getPosFromBuffer();
    setGlobalPos(pos);
    record.push_back({pos,t});
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