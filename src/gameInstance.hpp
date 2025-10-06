#pragma once
#include <chrono>
#include <memory>
#include <string>
#include "player.hpp"
#include "gameConstants.hpp"
#include "globals.hpp"

enum GameState {POSITIONING, START_RUNNING, RUNNING, TURN_ENDED, GAME_ENDED};
class GameInstance
{
private:
    
    std::string& wName;
    double& scale;
    bool& tryflip;
    cv::CascadeClassifier& cascade;
    cv::VideoCapture& capture;

    std::vector<std::shared_ptr<Player>> past;
    std::shared_ptr<Player> current;
    std::chrono::time_point<std::chrono::steady_clock> startTime;

    GameState state;

    void startPositioning();
    void updatePositioning();
    void startRunning();
    void updateRunning();

    void drawHiboxes(cv::Mat frame);
public:
    std::shared_ptr<Player> getCurrentPlayer();
    GameInstance(std::string& wName, double& scale, bool& tryflip,
        cv::CascadeClassifier& cascade, cv::VideoCapture& capture);

    std::shared_ptr<Player> createPlayer(std::string asset, std::string item_name);

    cv::Mat getFrame();
    std::vector<cv::Rect> getFaces(cv::Mat frame);
    
    void startTurn(int number, std::string item);
    void updateTurn();
    void endTurn();

    int getTimeFromStart();
    bool hasTurnEnded();
    bool hasGameEnded();
};