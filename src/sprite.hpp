#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "transform.hpp"
#include <string>

#define NO_SCALING 1

class Sprite : public Transform
{
protected:
    cv::Mat img;
public:
    Sprite(std::string asset);
    void draw(cv::Mat windowFrame);
    static bool applyScaleToImg(cv::Mat& toScale, std::pair<float,float> size);
    static bool applyRotationToImg(cv::Mat& toRotate, float rot);

    static void drawImageFromCenter(cv::Mat frame, cv::Mat img, int xPos, int yPos);
    static void drawImageFromConner(cv::Mat frame, cv::Mat img, int xPos, int yPos);
};