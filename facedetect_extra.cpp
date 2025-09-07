#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "src/transform.hpp"
#include <cstdio>
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDraw( Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip);

string cascadeName;
string wName = "Game";
void drawImage(Mat frame, Mat img, int xPos, int yPos);
class Orange : public Transform
{
public:
    std::pair<float, float> speed;
    
    string file;
    void update()
    {
        float r = getParent()->getLocalRotation();
        getParent()->setLocalRotation(r+0.1f);
        log();
    }
    void log()
    {
        pair<float, float> v;
        cout<<"Laranja: "<<endl;
        v = getLocalPos();
        cout<<" LocalPos: "<<v.first<<", "<<v.second<<endl;
        v = getGlobalPos();
        cout<<" globalPos: "<<v.first<<", "<<v.second<<endl;
        cout<<"Parent: "<<endl;
        v = getParent()->getLocalPos();
        cout<<" ParentLocalPos: "<<v.first<<", "<<v.second<<endl;
        v = getParent()->getGlobalPos();
        cout<<" ParentGlobalPos: "<<v.first<<", "<<v.second<<endl;
        cout<<endl;
    }
    void draw(Mat smallFrame)
    {
        // Desenha uma imagem
        Mat img = imread(file, IMREAD_UNCHANGED), img2;
        //printf("img::width: %d, height=%d\n", img.cols, img.rows );
        if (img.rows > 200 || img.cols > 200)
            resize( img, img, Size(200, 200));
        auto pos = getGlobalPos();
        drawImage(smallFrame, img, pos.first, pos.second);
    }
};

Orange* o;

int main( int argc, const char** argv )
{
    Transform* p = new Transform();
    o = new Orange();
    o->file = "orange.png";
    o->speed.first = 1;
    o->speed.second = 1;
    p->setLocalPos({300,300});
    o->setParent(p);
    o->setLocalPos({100,0});

    o->log();
    
    VideoCapture capture;
    Mat frame;
    bool tryflip;
    CascadeClassifier cascade;
    double scale;
    char key = 0;

    cascadeName = "haarcascade_frontalface_default.xml";
    scale = 2; // usar 1, 2, 4.
    if (scale < 1)
        scale = 1;
    tryflip = true;

    if (!cascade.load(cascadeName)) {
        cout << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
        return -1;
    }

    if(!capture.open("video.mp4")) // para testar com um video
    // if(!capture.open(0)) // para testar com a webcam
    {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    if( capture.isOpened() ) {
        cout << "Video capturing has been started ..." << endl;
        namedWindow(wName, WINDOW_KEEPRATIO);

        while (1)
        {
            capture >> frame;
            if( frame.empty() )
                break;
            if (key == 0) // just first time
                resizeWindow(wName, frame.cols/scale, frame.rows/scale);

            o->update();
            auto [x,y] = o->getGlobalPos();
    
            detectAndDraw( frame, cascade, scale, tryflip );

            key = (char)waitKey(10);
            if( key == 27 || key == 'q' || key == 'Q' )
                break;
        }
    }
    delete o;
    delete p;
    return 0;
}

/**
 * @brief Draws a transparent image over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */
void drawImage(Mat frame, Mat img, int xPos, int yPos) {
    if (yPos + img.rows >= frame.rows || xPos + img.cols >= frame.cols)
        return;

    Mat mask;
    vector<Mat> layers;

    split(img, layers); // seperate channels
    if (layers.size() == 4) { // img com transparencia.
        Mat rgb[3] = { layers[0],layers[1],layers[2] };
        mask = layers[3]; // png's alpha channel used as mask
        merge(rgb, 3, img);  // put together the RGB channels, now transp insn't transparent 
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols), mask);
    } else {
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols));
    }
}

/**
 * @brief Draws a transparent rect over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param color the color of the rect
 * @param alpha transparence level. 0 is 100% transparent, 1 is opaque.
 * @param regin rect region where the should be positioned
 */
void drawTransRect(Mat frame, Scalar color, double alpha, Rect region) {
    Mat roi = frame(region);
    Mat rectImg(roi.size(), CV_8UC3, color); 
    addWeighted(rectImg, alpha, roi, 1.0 - alpha , 0, roi); 
}

void detectAndDraw( Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip)
{
    vector<Rect> faces;
    Mat grayFrame, smallFrame;
    Scalar color = Scalar(255,0,0);

    double fx = 1 / scale;
    resize( frame, smallFrame, Size(), fx, fx, INTER_LINEAR_EXACT );
    if( tryflip )
        flip(smallFrame, smallFrame, 1);
    cvtColor( smallFrame, grayFrame, COLOR_BGR2GRAY );
    equalizeHist( grayFrame, grayFrame );

    //printf("smallFrame::width: %d, height=%d\n", smallFrame.cols, smallFrame.rows );

    cascade.detectMultiScale( grayFrame, faces,
        1.3, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(40, 40) );

    // PERCORRE AS FACES ENCONTRADAS
    for (Rect r : faces)
    {
        rectangle( smallFrame, Point(cvRound(r.x), cvRound(r.y)),
                    Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))),
                    color, 3);
    }

    o->draw(smallFrame);
    // Desenha quadrados com transparencia
    double alpha = 0.3; // entre 0 e 1
    drawTransRect(smallFrame, Scalar(0,255,0), alpha, Rect(  0, 0, 200, 200));
    drawTransRect(smallFrame, Scalar(255,0,0), alpha, Rect(200, 0, 200, 200));

    // Desenha um texto
    color = Scalar(0,0,255);
    putText	(smallFrame, "Placar:", Point(300, 50), FONT_HERSHEY_PLAIN, 2, color); // fonte

    // Desenha o frame na tela
    imshow(wName, smallFrame);
}
