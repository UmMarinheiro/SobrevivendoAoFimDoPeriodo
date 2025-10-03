#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "transform.hpp"
#include "upperConnerTransform.hpp"
#include <memory>
#include <string>
#include <vector>
#include "gameConstants.hpp"

class Colisor : public UpperConnerTransform
{
private:
    std::string identifier;
    Colisor(std::string identifier, Transform* intitParent = nullptr);
public:
    static std::shared_ptr<Colisor> createColisor(std::string identifier, Transform* intitParent = nullptr);

    std::string getIdentifier() ;
    void setIdentifier(std::string identifier);
    std::vector<std::string> getColisions();
    std::vector<std::string> getColisionsStartingWith(std::string filter);
    bool isPointInside(std::pair<float,float> point);
    bool isCollidingWith(Colisor* other) ;
    cv::Rect getRect() ;
};