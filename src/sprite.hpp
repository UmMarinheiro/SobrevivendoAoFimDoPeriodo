#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "transform.hpp"
#include "upperConnerTransform.hpp"
#include <memory>
#include <string>

#define NO_SCALING 1
#define HORIZONTAL_FLIP 1
#define VERTICAL_FLIP 0
#define MINIMUM_SIZE 1

class Sprite : public UpperConnerTransform
{
private:
    cv::Mat img;
    bool isVisible = true;
    int layer;
    Sprite(std::string asset, Transform* intitParent = nullptr);
    Sprite(cv::Mat img, Transform* intitParent = nullptr);
public:
    
    static std::shared_ptr<Sprite> createSprite(std::string asset,Transform* intitParent = nullptr);
    static std::shared_ptr<Sprite> createSprite(cv::Mat img, Transform* intitParent = nullptr);

    void draw(cv::Mat& windowFrame);

    bool getVisibility();
    void setVisibility(bool newVisibility);
    int getLayer();
    void setLayer(int layer);

    cv::Mat getImg();
    cv::Mat getImgRef();
    void changeImg(std::string asset);
    void changeImg(cv::Mat img);

    static bool applyScaleToImg(cv::Mat& toScale, std::pair<float,float> size);

    // NÃO FUNCIONA -> Falta atualizar o tamanho da imagem rotacionada
    static bool applyRotationToImg(cv::Mat& toRotate, float rot);

    static void drawImage(cv::Mat& frame, cv::Mat img, int xPos, int yPos);
};