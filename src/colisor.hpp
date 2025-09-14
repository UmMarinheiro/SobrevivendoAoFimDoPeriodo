#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "transform.hpp"
#include <memory>
#include <string>
#include <vector>

#define MIN_PIXEL_FOR_COLISION 10

class Colisor : public Transform
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
    bool isCollidingWith( Colisor* other) ;
    cv::Rect getRect() ;
};