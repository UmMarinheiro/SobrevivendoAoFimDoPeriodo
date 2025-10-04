#include "colisorMan.hpp"
#include "gameInstance.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "Menu.hpp"
#include "gameUtils.hpp" 
#include <iostream>
#include "opencvUtils.hpp"
#include "sprite.hpp"
#include "colisor.hpp"
#include "spriteMan.hpp"
#include "transform.hpp"
#include <memory>
#include "globals.hpp"
#include <chrono>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include "globals.hpp"

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

extern int lastCoordX;
extern int lastCoordY;

string ImagePath = "assets/items/Mochila/imagem.png";

void drawItem(Mat frame, Mat img, int xPos, int yPos){
    Mat itemImage = imread(imagePath, IMREAD_UNCHANGED);
    
    if (!itemImage.empty()) {
        int maxSize = 300;
        if (itemImage.cols > maxSize || itemImage.rows > maxSize) {
            double scale = min((double)maxSize / itemImage.cols, (double)maxSize / itemImage.rows);
            resize(itemImage, itemImage, Size(), scale, scale, INTER_LINEAR);
        }
        
        drawImage(frame, itemImage, lastCoordX, lastCoordY);
    } else {
        string errorMsg = "Item: " + itemName;
        putText(frame, errorMsg, Point(50, 240), 
                FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);
    }
    
    imshow(wName, frame); 
}