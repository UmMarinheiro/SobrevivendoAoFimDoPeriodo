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

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

string ImagePath = "assets/items/Marmita/imagem.png";

void drawItem(Mat frame, Mat img, int xPos, int yPos){
    
}