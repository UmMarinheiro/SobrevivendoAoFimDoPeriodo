#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <string>

using namespace cv;

class Render{

    private:

    Mat frame, orange;
    CascadeClassifier cascade;

    std::string wName = "Game";

    public:

    Render(){}

    void detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip, Mat& orange);
    void overlayImage(Mat &background, Mat &foreground, Point location);

    void drawImage(Mat frame, Mat img, int xPos, int yPos){}

};