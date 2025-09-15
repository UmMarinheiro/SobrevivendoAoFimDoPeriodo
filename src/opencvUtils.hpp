#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

cv::Mat getFrame(cv::Mat baseFrame, double scale, bool tryflip);
std::vector<cv::Rect> getFaces(cv::Mat frame, cv::CascadeClassifier& cascade);