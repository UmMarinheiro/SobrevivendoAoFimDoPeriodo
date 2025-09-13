#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "transform.hpp"
#include <string>

#define NO_SCALING 1
#define HORIZONTAL_FLIP 1
#define VERTICAL_FLIP 0
#define MINIMUM_SIZE 1

class Sprite : public Transform
{
private:
    cv::Mat img;
    bool isVisible = true;
public:
    Sprite(std::string asset);
    Sprite(cv::Mat img);
    void draw(cv::Mat windowFrame);

    bool getVisibility();
    void setVisibility(bool newVisibility);

    cv::Mat getImg();
    cv::Mat getImgRef();
    void changeImg(std::string asset);
    void changeImg(cv::Mat img);

    static bool applyScaleToImg(cv::Mat& toScale, std::pair<float,float> size);

    // NÃO FUNCIONA -> Falta atualizar o tamanho da imagem rotacionada
    static bool applyRotationToImg(cv::Mat& toRotate, float rot);

    static void drawImageFromCenter(cv::Mat frame, cv::Mat img, int xPos, int yPos);
    static void drawImageFromConner(cv::Mat frame, cv::Mat img, int xPos, int yPos);
};