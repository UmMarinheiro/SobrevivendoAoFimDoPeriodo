#include "transform.hpp"
#include <cmath>
#include <utility>
using namespace std;


Transform* Transform::getParent(){return parent;}
void Transform::setParent(Transform *parent)
{
    std::pair<float, float> globalPos = getGlobalPos();
    this->parent = parent;
    
    setGlobalPos(globalPos);
}

std::pair<float, float > Transform::getLocalPos() {return this->pos;}
std::pair<float, float > Transform::getLocalSize() {return this->size;}
float Transform::getLocalRotation() {return this->rotation;}

void Transform::setLocalPos(std::pair<float, float> localPos) {this->pos = localPos;}
void Transform::setLocalSize(std::pair<float, float> localSize) {this->size = localSize;}
void Transform::setLocalRotation(float localRotation) {this->rotation = localRotation;}

std::pair<float, float > Transform::getGlobalPos(){return posLocalToGlobal(IDENTITY_POS);}
std::pair<float, float > Transform::getGlobalSize(){return sizeLocalToGlobal(IDENTITY_SIZE);}
float Transform::getGlobalRotation(){return rotationGlobalToLocal(IDENTITY_ROTATION);}

void Transform::setGlobalPos(std::pair<float, float> globalPos) 
{
    if(parent == nullptr) this->pos = globalPos;
    else this->pos = parent->posGlobalToLocal(globalPos);
}
void Transform::setGlobalSize(std::pair<float, float> globalSize) 
{
    if(parent == nullptr) this->size = globalSize;
    else this->size = parent->sizeGlobalToLocal(globalSize);
}
void Transform::setGlobalRotation(float globalRotation) 
{
    if(parent == nullptr) this->rotation = globalRotation;
    else this->rotation = parent->rotationGlobalToLocal(globalRotation);
}

void Transform::translate(std::pair<float, float> translation) 
{
    std::pair<float, float> localPos = getLocalPos();
    localPos.first += translation.first;
    localPos.second += translation.second;
    
    setLocalPos(localPos);
}
void Transform::rotate(float angle)
{
    setLocalRotation(getLocalRotation() + angle);
}

std::pair<float, float> Transform::posLocalToGlobal(std::pair<float, float> localPos) 
{
    if(parent == nullptr) return posLocalToParent(localPos); //MELHORAR
    
    Transform &parentRef = *parent;
    std::pair<float, float> posInParent = posLocalToParent(localPos);
    
    return parentRef.posLocalToGlobal(posInParent);
}
std::pair<float, float> Transform::posGlobalToLocal(std::pair<float, float> globalPos) 
{
    if(parent == nullptr) return posParentToLocal(globalPos); //MELHORAR

    Transform &parentRef = *parent;
    std::pair<float, float> posInParent = parentRef.posGlobalToLocal(globalPos);
    pair<float, float> localPos = posParentToLocal(posInParent);
    return localPos;
}


std::pair<float, float> Transform::sizeLocalToGlobal(std::pair<float, float> localSize) 
{
    if(parent == nullptr) return sizeLocalToParent(localSize); //MELHORAR
    
    Transform &parentRef = *parent;
    std::pair<float, float> sizeInParent = sizeLocalToParent(localSize);
    
    return parentRef.sizeLocalToGlobal(sizeInParent);
}
std::pair<float, float> Transform::sizeGlobalToLocal(std::pair<float, float> globalSize) 
{
    if(parent == nullptr) return sizeParentToLocal(globalSize); //MELHORAR
    
    Transform &parentRef = *parent;
    std::pair<float, float> sizeInParent = parentRef.sizeGlobalToLocal(globalSize);
    pair<float, float> localSize = sizeParentToLocal(sizeInParent);
    return localSize;
}

float Transform::rotationGlobalToLocal(float globalRotation) 
{
    if(parent == nullptr) return rotationParentToLocal(globalRotation); //MELHORAR
    
    Transform &parentRef = *parent;
    float rotationInParent = parentRef.rotationGlobalToLocal(globalRotation);
    float localRotation = rotationParentToLocal(rotationInParent);
    return localRotation;
}
float Transform::rotationLocalToGlobal(float localRotation) 
{   
    if(parent == nullptr) return rotationLocalToParent(localRotation); //MELHORAR
    
    Transform &parentRef = *parent;
    float rotationInParent = rotationLocalToParent(localRotation);
    
    return parentRef.rotationLocalToGlobal(rotationInParent);
}



std::pair<float, float> Transform::applyTranslation(std::pair<float, float> origin, std::pair<float, float> vec) 
{
    return {
        vec.first + origin.first, 
        vec.second + origin.second
    };
}
std::pair<float, float> Transform::applyScaling(std::pair<float, float> size, std::pair<float, float> vec) 
{
    return {
        vec.first * size.first, 
        vec.second * size.second
    };
}
std::pair<float, float> Transform::applyRotation(float rotation, std::pair<float, float> vec) 
{
    float sinRot = std::sin(M_PI * rotation/180);
    float cosRot = std::cos(M_PI * rotation/180);
    
    return {
        vec.first * cosRot - vec.second * sinRot, 
        vec.first * sinRot + vec.second * cosRot};
    }
    
std::pair<float, float> Transform::removeTranslation(std::pair<float, float> origin, std::pair<float, float> vec) 
{
    return {
        vec.first - origin.first, 
        vec.second - origin.second
    };
}
std::pair<float, float> Transform::removeScaling(std::pair<float, float> size, std::pair<float, float> vec) 
{
    return {
        vec.first / size.first, 
        vec.second / size.second
    };
}
std::pair<float, float> Transform::removeRotation(float rotation, std::pair<float, float> vec) 
{
    float sinRot = std::sin(M_PI * rotation/180);
    float cosRot = std::cos(M_PI * rotation/180);
    
    return {
        vec.first * cosRot + vec.second * sinRot,
        - vec.first * sinRot + vec.second * cosRot,
    };
}

std::pair<float, float> Transform::posLocalToParent(std::pair<float, float> localPos) 
{
    return 
        applyTranslation(pos, 
        applyScaling(size, 
        applyRotation(rotation, 
            
            localPos
            )));
}
std::pair<float, float> Transform::posParentToLocal(std::pair<float, float> parentPos) 
{
    return 
        removeRotation(rotation, 
        removeScaling(size, 
        removeTranslation(pos, 

            parentPos
        )));
}

std::pair<float, float> Transform::sizeLocalToParent(std::pair<float, float> localSize) 
{
    return {
        localSize.first * size.first,
        localSize.second * size.second
    };
}
std::pair<float, float> Transform::sizeParentToLocal(std::pair<float, float> parentSize) 
{
    return {
        parentSize.first / size.first,
        parentSize.second / size.second
    };
}

float Transform::rotationLocalToParent(float localRotation) 
{
    return localRotation + rotation;
}
float Transform::rotationParentToLocal(float parentRotation) 
{
    return parentRotation - rotation;
}
