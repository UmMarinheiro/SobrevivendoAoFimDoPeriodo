#pragma once
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <chrono>

class AnimatedSprite {
private:
    int currentFrame;
    double frameDuration;
    std::chrono::steady_clock::time_point lastUpdate;
    bool isPlaying;
    bool loop;
    bool shouldAnimate;

public:
    std::vector<cv::Mat> frames;
    
    // Construtor
    AnimatedSprite();
    
    // Declarações dos métodos (sem implementação)
    void loadFrames(const std::vector<std::string>& framePaths);
    void generatePulseFrames(int width, int height, cv::Scalar baseColor, int frameCount);
    void update();
    void startAnimation();
    void stopAnimation();
    cv::Mat getCurrentFrame();
    void setFrameDuration(double duration);
    bool isAnimationPlaying() const;
    bool shouldAnimateNow() const;
};

// Estrutura para botões animados
struct AnimatedButton {
    cv::Rect rect;
    std::string text;
    cv::Mat staticFrame;
    AnimatedSprite hoverAnimation;
    bool isHovered;
    bool wasHovered;
    
    // Construtor
    AnimatedButton(cv::Rect r, std::string t);
    
    // Declarações dos métodos
    void setStaticFrame(cv::Scalar color);
    void updateAnimation();
    cv::Mat getCurrentSprite();
};