#include "upperConnerTransform.hpp"
#include "transform.hpp"
#include <utility>

UpperConnerTransform::UpperConnerTransform(Transform *parent, std::pair<float, float> pos, 
    std::pair<float, float> size,float rotation) : Transform::Transform(parent, pos, size, rotation){}
    
std::pair<float, float> UpperConnerTransform::getUpperConner()
{
    std::pair<float, float> size = getLocalSize();
    return {-size.first/2, -size.second/2};
}
void UpperConnerTransform::setLocalPos(std::pair<float, float> localPos)
{
    this->pos = Transform::applyTranslation(localPos, getUpperConner());
}
void UpperConnerTransform::setLocalSize(std::pair<float, float> localSize)
{
    this->size = localSize;
    setLocalPos({0,0});
}
