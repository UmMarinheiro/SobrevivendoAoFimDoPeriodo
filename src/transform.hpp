#include <utility>

#define IDENTITY_POS {0,0}
#define IDENTITY_SIZE {1,1}
#define IDENTITY_ROTATION 0

class Transform
{
private:
    Transform *parent = nullptr; // SUBSTITUIR POR SMART_POINTER
    std::pair<float, float> pos = IDENTITY_POS;
    std::pair<float, float> size = IDENTITY_SIZE;
    float rotation = IDENTITY_ROTATION;
    
public:
    Transform *getParent();
    void setParent(Transform *parent);

    std::pair<float, float > getLocalPos();
    std::pair<float, float > getLocalSize();
    float getLocalRotation();

    std::pair<float, float > getGlobalPos();
    std::pair<float, float > getGlobalSize();
    float getGlobalRotation();

    void setLocalPos(std::pair<float, float> localPos);
    void setLocalSize(std::pair<float, float> localSize);
    void setLocalRotation(float lovalRotation);
    
    void setGlobalPos(std::pair<float, float> globalPos);
    void setGlobalSize(std::pair<float, float> globalSize);
    void setGlobalRotation(float globalRotation);
    
    void translate(std::pair<float, float> translation);

    std::pair<float, float> posLocalToGlobal(std::pair<float, float> localPos);
    std::pair<float, float> sizeLocalToGlobal(std::pair<float, float> localSize);
    float rotationLocalToGlobal(float localRotation);

    std::pair<float, float> posGlobalToLocal(std::pair<float, float> globalPos);
    std::pair<float, float> sizeGlobalToLocal(std::pair<float, float> globalSize);
    float rotationGlobalToLocal(float globalRotation);

    std::pair<float, float> posLocalToParent(std::pair<float, float> localPos);
    std::pair<float, float> posParentToLocal(std::pair<float, float> parentPos);

    std::pair<float, float> sizeLocalToParent(std::pair<float, float> localSize); 
    std::pair<float, float> sizeParentToLocal(std::pair<float, float> parentSize); 

    float rotationParentToLocal(float parentRotation); 
    float rotationLocalToParent(float localRotation);

    static std::pair<float, float> applyTranslation(std::pair<float, float> origin, std::pair<float, float> vec);
    static std::pair<float, float> applyScaling(std::pair<float, float> size, std::pair<float, float> vec);
    static std::pair<float, float> applyRotation(float rotation, std::pair<float, float> vec);

    static std::pair<float, float> removeTranslation(std::pair<float, float> origin, std::pair<float, float> vec);
    static std::pair<float, float> removeScaling(std::pair<float, float> size, std::pair<float, float> vec);
    static std::pair<float, float> removeRotation(float rotation, std::pair<float, float> vec);
};