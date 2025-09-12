#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "Menu.hpp" 
#include "Render.hpp"
#include "globals.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;


//void Render::detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip, Mat& orange);
//void Render::overlayImage(Mat &background, Mat &foreground, Point location);

void Render::drawImage(Mat frame, Mat img, int xPos, int yPos) {
    if (yPos + img.rows >= frame.rows || xPos + img.cols >= frame.cols || xPos <= 0 || yPos <= 0)
        return;

    Mat mask;
    vector<Mat> layers;

    split(img, layers);
    if (layers.size() == 4) {
        Mat rgb[3] = { layers[0],layers[1],layers[2] };
        mask = layers[3];
        merge(rgb, 3, img);
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols), mask);
    } else {
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols));
    }
}

void Render::detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip, Mat& orange)
{
    vector<Rect> faces;
    Mat smallFrame, grayFrame;

    double fx = 1.0 / scale;
    resize(frame, smallFrame, Size(), fx, fx, INTER_LINEAR_EXACT);
    if (tryflip)
        flip(smallFrame, smallFrame, 1);
    cvtColor(smallFrame, grayFrame, COLOR_BGR2GRAY);
    equalizeHist(grayFrame, grayFrame);

    cascade.detectMultiScale(grayFrame, faces,
        1.3, 2, 0 | CASCADE_SCALE_IMAGE, Size(40, 40));

        static int mopa = 1;
        static int tito = 5;
    
        Mat img = imread("orange.png", IMREAD_UNCHANGED), img2;
        printf("img::width: %d, height=%d\n", img.cols, img.rows );
        if (img.rows > 200 || img.cols > 200)
            resize( img, img, Size(200, 200));
        drawImage(smallFrame, img, mopa += tito, 300);
        Rect recLaranja = Rect(mopa, 300, img.cols, img.rows);
        if(mopa > smallFrame.cols - img.cols){
            tito = -tito;
        }

    // percorre as faces encontradas
    for (Rect r : faces) {

        if((r & recLaranja).area() > 500){
            Scalar(0,0,255);
        }else{
            Scalar(255,0,0);
            rectangle( smallFrame, Point(cvRound(r.x), cvRound(r.y)),
                    Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))),
                    Scalar(0,0,255), 3);
        }

        Mat orange_resized;
        resize(orange, orange_resized, Size(r.width, r.height));
        overlayImage(smallFrame, orange_resized, Point(r.x, r.y));
    }

    imshow(wName, smallFrame);
}
void Render::overlayImage(Mat &background, Mat &foreground, Point location) {
    for (int y = 0; y < foreground.rows; ++y) {
        for (int x = 0; x < foreground.cols; ++x) {
            Vec4b fgPixel = foreground.at<Vec4b>(y, x);
            int alpha = fgPixel[3];
            if (alpha > 0) {
                int bx = location.x + x;
                int by = location.y + y;
                if (bx >= 0 && bx < background.cols && by >= 0 && by < background.rows) {
                    Vec3b &bgPixel = background.at<Vec3b>(by, bx);
                    for (int c = 0; c < 3; ++c) {
                        bgPixel[c] = (bgPixel[c] * (255 - alpha) + fgPixel[c] * alpha) / 255;
                    }
                }
            }
        }
    }
}