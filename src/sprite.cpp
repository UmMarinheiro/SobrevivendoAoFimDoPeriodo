#include <memory>
#include <utility>
#include "sprite.hpp"
#include "spriteMan.hpp"
#include "transform.hpp"
#include "upperConnerTransform.hpp"

Sprite::Sprite(std::string asset, Transform* intitParent) : UpperConnerTransform(intitParent) {changeImg(asset);}
Sprite::Sprite(cv::Mat img, Transform* intitParent) : UpperConnerTransform(intitParent) {changeImg(img);}

std::shared_ptr<Sprite> Sprite::createSprite(std::string asset, Transform* intitParent)
{
    auto sprite_sptr = std::make_shared<Sprite>(Sprite(asset, intitParent));
    SpriteMan::addSprite(sprite_sptr);
    return sprite_sptr;
}
std::shared_ptr<Sprite> Sprite::createSprite(cv::Mat img, Transform* intitParent)
{
    auto sprite_sptr = std::make_shared<Sprite>(Sprite(img, intitParent));
    SpriteMan::addSprite(sprite_sptr);
    return sprite_sptr;
}

void Sprite::draw(cv::Mat& windowFrame)
{
    if(!isVisible)return;
    std::pair<float,float> pos = getGlobalPos();
    std::pair<float,float> size = getGlobalSize();
    //float rot = getGlobalRotation();

    cv::Mat imgToDraw = img.clone();
    if(!applyScaleToImg(imgToDraw, size))return;
    //if(!applyRotationToImg(imgToDraw, rot))return;
    drawImage(windowFrame, imgToDraw, pos.first, pos.second);
}


bool Sprite::getVisibility() {return isVisible;}
void Sprite::setVisibility(bool newVisibility) {this->isVisible = newVisibility;}

cv::Mat Sprite::getImg() {return img.clone();}
cv::Mat Sprite::getImgRef() {return img;}
void Sprite::changeImg(std::string asset) 
{
    this->img = imread(asset, cv::IMREAD_UNCHANGED);
    setLocalSize({img.cols, img.rows});
}
void Sprite::changeImg(cv::Mat img) 
{
    this->img = img.clone();
    setLocalSize({img.cols, img.rows});
}

bool Sprite::applyScaleToImg(cv::Mat& toScale, std::pair<float,float> size)
{
    std::pair<float, float> newSize = {std::abs(size.first), std::abs(size.second)};
    if(newSize.first < MINIMUM_SIZE || newSize.second < MINIMUM_SIZE) return false;
    
    if(size.first < 0) flip(toScale, toScale, HORIZONTAL_FLIP);
    if(size.second < 0) flip(toScale, toScale, VERTICAL_FLIP);
    resize(toScale, toScale, cv::Size(newSize.first, newSize.second));
    return true;
}

// NÃO FUNCIONA -> Falta atualizar o tamanho da imagem rotacionada
bool Sprite::applyRotationToImg(cv::Mat& toRotate, float rot) 
{
    int imgWidth = toRotate.cols;
    int imgHeight = toRotate.rows;
    
    cv::Point2f imgCenter(imgWidth / 2.0f, imgHeight / 2.0f);    
    cv::Mat rotation_matrix = cv::getRotationMatrix2D(imgCenter, -rot, NO_SCALING);
    
    cv::warpAffine(toRotate, toRotate, rotation_matrix, cv::Size(imgWidth, imgHeight));
    
    return true;
}

void Sprite::drawImage(cv::Mat& frame, cv::Mat img, int xPos, int yPos) 
{
    if(yPos <= 0 || xPos <= 0 ||
        yPos + img.rows >= frame.rows || xPos + img.cols >= frame.cols)
        return;

    cv::Mat mask;
    std::vector<cv::Mat> layers;

    split(img, layers); // seperate channels
    if (layers.size() == 4) { // img com transparencia.
        cv::Mat rgb[3] = { layers[0],layers[1],layers[2] };
        mask = layers[3]; // png's alpha channel used as mask
        merge(rgb, 3, img);  // put together the RGB channels, now transp insn't transparent 
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols), mask);
    } else {
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols));
    }
}