#include "gameInstance.hpp"
#include "opencvUtils.hpp"
#include "player.hpp"
#include "spriteMan.hpp"
#include <chrono>
#include <memory>
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

void GameInstance::startTurn(int number, int item)
{
    current = Player::createPlayer("assets/players/player" + std::to_string(number) + ".png", item);
    startPositioning();
}
void GameInstance::updateTurn()
{
    switch (state) 
    {
        case POSITIONING:
            updatePositioning();
            break;
        case START_RUNNING:
            startRunning();
        case RUNNING:
            updateRunning();
            break;
        default:
            return;
    }
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
void GameInstance::startPositioning()
{
    current->startPositioningRec();
    for(auto played : past) played->startPositioningPast();

    startTime = std::chrono::steady_clock::now();
    state = POSITIONING;
}
void GameInstance::updatePositioning()
{
    cv::Mat frame = getFrame();
    std::vector<cv::Rect> faces = getFaces(frame);
    int elapsedTime = getTimeFromStart();

    if(faces.size() > 0) 
    {
        std::pair<float, float> pos = getRectCenter(faces[0]);
        current->updatePositioningRec(&pos);
    }
    else current->updatePositioningRec(nullptr);
    for(auto played : past) played->updatePositioningPast();

    SpriteMan::windowFrame = frame;
    SpriteMan::tick();

    cv::imshow(wName, frame);

    if(elapsedTime > PREPARE_DURATION_MS) state = START_RUNNING;
}
void GameInstance::startRunning()
{
    current->startRec();
    for(auto played : past) played->startPast();

    startTime = std::chrono::steady_clock::now();
    state = RUNNING;
}
void GameInstance::updateRunning()
{
    cv::Mat frame = getFrame();
    std::vector<cv::Rect> faces = getFaces(frame);
    int elapsedTime = getTimeFromStart();

    if(faces.size() > 0) 
    {
        std::pair<float, float> pos = getRectCenter(faces[0]);
        current->updateRec(&pos, elapsedTime);
    }
    else current->updateRec(nullptr, elapsedTime);
    for(auto played : past) played->updatePast(elapsedTime);

    SpriteMan::windowFrame = frame;
    SpriteMan::tick();

    cv::imshow(wName, frame);

    if(!current->isAlive()) state = GAME_ENDED;
    else if(elapsedTime > TURN_DURATION_MS)state = TURN_ENDED;
}


int GameInstance::getTimeFromStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
}
bool GameInstance::hasTurnEnded() {return state == TURN_ENDED;}
bool GameInstance::hasGameEnded() {return state == GAME_ENDED;}