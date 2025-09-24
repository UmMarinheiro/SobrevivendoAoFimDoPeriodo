#include "gameInstance.hpp"
#include "opencvUtils.hpp"
#include "player.hpp"
#include "spriteMan.hpp"
#include <chrono>
#include <string>

GameInstance::GameInstance(std::string& wName, double& scale, bool& tryflip,
        cv::CascadeClassifier& cascade, cv::VideoCapture& capture) 
    : wName(wName), scale(scale), tryflip(tryflip), cascade(cascade), capture(capture) {}

cv::Mat GameInstance::getFrame()
{
    cv::Mat baseFrame;
    capture >> baseFrame;
    return ::getFrame(baseFrame, scale, tryflip);
}
std::vector<cv::Rect> GameInstance::getFaces(cv::Mat frame)
{
    return ::getFaces(frame, cascade);
}

void GameInstance::startTurn(int number)
{
    ended = false;
    current = std::make_shared<Player>("assets/players/player" + std::to_string(number) + ".png");
    
    current->startRec();
    for(auto played : past) played->startPast();

    startTime = std::chrono::steady_clock::now();
}
void GameInstance::updateTurn()
{
    cv::Mat frame = getFrame();
    std::vector<cv::Rect> faces = getFaces(frame);
    int elapsedTime = getTimeFromStart();

    if(faces.size() > 0) current->updateRec(getRectCenter(faces[0]), elapsedTime);
    for(auto played : past) played->updatePast(elapsedTime);

    SpriteMan::windowFrame = frame;
    SpriteMan::tick();

    cv::imshow(wName, frame);
    if(elapsedTime > TURN_DURATION_MS) ended = true;
}
void GameInstance::endTurn()
{
    for(auto played : past)played->endPast();
    
    if(current!=nullptr) 
    {
        current->endRec();
        past.push_back(current);
    }
    current = nullptr;
}
int GameInstance::getTimeFromStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
}
bool GameInstance::hasTurnEnded() {return ended;}