#include "opencvUtils.hpp"
#include <vector>

cv::Mat getFrame(cv::Mat baseFrame, double scale, bool tryflip)
{
    cv::Mat ajustedFrame;
    double fx = 1.0 / scale;
    resize(baseFrame, ajustedFrame, cv::Size(), fx, fx, cv::INTER_LINEAR_EXACT);
    if (tryflip)
        flip(ajustedFrame, ajustedFrame, 1);
    return ajustedFrame;
}
std::vector<cv::Rect> getFaces(cv::Mat frame, cv::CascadeClassifier& cascade)
{
    cv::Mat grayFrame;
    
    cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    equalizeHist(grayFrame, grayFrame);
    
    std::vector<cv::Rect> faces;

    cascade.detectMultiScale(grayFrame, faces,
        1.3, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(40, 40));

    return faces;
}