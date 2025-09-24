#include "player.hpp"
#include <utility>

std::pair<float, float> getRectCenter(cv::Rect r) {return {r.x + r.width/2, r.y + r.height/2};}

Player::Player(std::string asset)
{
    sprite_sptr = Sprite::createSprite(asset, (Transform*)this);
    sprite_sptr->setLocalSize({1,1});
    setLocalSize({50,50});
}

void Player::startRec()
{

}
void Player::startPast()
{

}
void Player::updateRec(std::pair<float, float> pos, float t)
{
    buffer[current] = pos;

    current++;
    if(current>=8)filled=true;
    current%=BUFFERSIZE;

    std::pair<float, float> val = {0,0};
    for(int i = 0; i < (filled?BUFFERSIZE:current); i++)
    {
        val.first += buffer[i].first;
        val.second += buffer[i].second;
    }
    val.first/=(filled?BUFFERSIZE:current);
    val.second/=(filled?BUFFERSIZE:current);
    setGlobalPos(val);
    record.push_back({val,t});
}
void Player::updatePast(float t)
{
    int i = 0;
    for(i = 0; i < record.size(); i++) 
        if(record[i].t > t) break;
    i--;
    if(i == -1) i = 0;

    setGlobalPos(record[i].pos);
}
void Player::endRec()
{

}
void Player::endPast()
{

}