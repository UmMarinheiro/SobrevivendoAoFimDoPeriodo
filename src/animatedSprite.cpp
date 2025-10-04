#include "animatedSprite.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <chrono>

AnimatedSprite::AnimatedSprite() : currentFrame(0), frameDuration(0.1), isPlaying(false), loop(true), shouldAnimate(false) {}

void AnimatedSprite::loadFrames(const std::vector<std::string>& framePaths) {
    frames.clear();
    for (const auto& path : framePaths) {
        cv::Mat frame = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (!frame.empty()) {
            frames.push_back(frame);
        }
    }
    lastUpdate = std::chrono::steady_clock::now();
}

void AnimatedSprite::generatePulseFrames(int width, int height, cv::Scalar baseColor, int frameCount) {
    frames.clear();
    for (int i = 0; i < frameCount; i++) {
        cv::Mat frame(height, width, CV_8UC3);
        
        // Efeito de pulsação
        double pulse;
        if (frameCount == 1) {
            pulse = 1.0; // Frame estático
        } else {
            pulse = 0.7 + 0.3 * sin(i * 0.6); // Animação
        }
        
        cv::Scalar color = baseColor * pulse;
        frame = cv::Mat(height, width, CV_8UC3, color);
        
        // Borda
        int borderThickness = frameCount == 1 ? 2 : (1 + (i % 3));
        cv::rectangle(frame, cv::Point(0, 0), cv::Point(width-1, height-1), 
                     cv::Scalar(90, 50, 30), borderThickness);
        
        frames.push_back(frame);
    }
}

void AnimatedSprite::update() {
    if (!shouldAnimate || frames.empty()) return;
    
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - lastUpdate).count();
    
    if (elapsed >= frameDuration) {
        currentFrame++;
        if (currentFrame >= frames.size()) {
            if (loop) {
                currentFrame = 0;
            } else {
                currentFrame = frames.size() - 1;
                isPlaying = false;
                shouldAnimate = false;
            }
        }
        lastUpdate = now;
    }
}

void AnimatedSprite::startAnimation() {
    shouldAnimate = true;
    isPlaying = true;
    currentFrame = 0;
    lastUpdate = std::chrono::steady_clock::now();
}

void AnimatedSprite::stopAnimation() {
    shouldAnimate = false;
    isPlaying = false;
    currentFrame = 0;
}

cv::Mat AnimatedSprite::getCurrentFrame() {
    if (frames.empty()) {
        cv::Mat fallback(100, 200, CV_8UC3, cv::Scalar(100, 100, 100));
        cv::rectangle(fallback, cv::Point(0, 0), cv::Point(199, 99), cv::Scalar(200, 200, 200), 2);
        return fallback;
    }
    return frames[currentFrame].clone();
}

void AnimatedSprite::setFrameDuration(double duration) { 
    frameDuration = duration; 
}

bool AnimatedSprite::isAnimationPlaying() const { 
    return isPlaying; 
}

bool AnimatedSprite::shouldAnimateNow() const { 
    return shouldAnimate; 
}

AnimatedButton::AnimatedButton(cv::Rect r, std::string t) 
    : rect(r), text(t), isHovered(false), wasHovered(false) {
    // Criar frame estático padrão
    staticFrame = cv::Mat(rect.height, rect.width, CV_8UC3, cv::Scalar(80, 80, 80));
    cv::rectangle(staticFrame, cv::Point(0, 0), cv::Point(rect.width-1, rect.height-1), 
                 cv::Scalar(200, 200, 200), 2);
}

void AnimatedButton::setStaticFrame(cv::Scalar color) {
    staticFrame = cv::Mat(rect.height, rect.width, CV_8UC3, color);
    cv::rectangle(staticFrame, cv::Point(0, 0), cv::Point(rect.width-1, rect.height-1), 
                 cv::Scalar(90, 50, 30), 2);
}

void AnimatedButton::updateAnimation() { // Controle de transição de animação
    
    if (isHovered && !wasHovered) { 
        hoverAnimation.startAnimation();
    } else if (!isHovered && wasHovered) {
        hoverAnimation.stopAnimation();
    }
    
    if (isHovered) {
        hoverAnimation.update();
    }
    
    wasHovered = isHovered;
}

cv::Mat AnimatedButton::getCurrentSprite() {
    if (isHovered) {
        cv::Mat sprite = hoverAnimation.getCurrentFrame();
        if (sprite.cols != rect.width || sprite.rows != rect.height) {
            cv::resize(sprite, sprite, cv::Size(rect.width, rect.height));
        }
        return sprite;
    } else {
        return staticFrame.clone();
    }
}