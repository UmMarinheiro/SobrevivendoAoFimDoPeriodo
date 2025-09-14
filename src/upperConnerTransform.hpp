#pragma once
#include "transform.hpp"
class UpperConnerTransform : public Transform
{
public:
    UpperConnerTransform(Transform *parent = nullptr, 
        std::pair<float, float> pos = IDENTITY_POS, 
        std::pair<float, float> size = IDENTITY_SIZE,
        float rotation = IDENTITY_ROTATION);

    std::pair<float, float> getUpperConner();
    void setLocalPos(std::pair<float, float> localPos);
    void setLocalSize(std::pair<float, float> localSize);
};