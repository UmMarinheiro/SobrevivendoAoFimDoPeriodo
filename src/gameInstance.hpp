#pragma once
#include <chrono>
#include <memory>
#include "player.hpp"

#define TURN_DURATION_MS 10000

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

    bool ended = false;

public:
    GameInstance(std::string& wName, double& scale, bool& tryflip,
        cv::CascadeClassifier& cascade, cv::VideoCapture& capture);

    cv::Mat getFrame();
    std::vector<cv::Rect> getFaces(cv::Mat frame);
    void startTurn(int number);
    void tick();
    int getTimeFromStart();
    bool hasTurnEnded();
};