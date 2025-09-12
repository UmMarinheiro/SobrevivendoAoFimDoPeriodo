#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "Menu.hpp" 
#include "Render.hpp"
#include "globals.hpp"
#include <iostream>

using namespace std;
using namespace cv;

// --- Estado do jogo (definição da variável global) ---
Estado estado = MENU;
Render render;

string cascadeName;
string wName = "Game";
bool tryflip = true;
double scale = 1;

CascadeClassifier cascade;
Mat orange;
Mat img;
VideoCapture capture;

float axys = 0.0;

int main()
{
    //--- Abrir câmera ---
    if(!capture.open("video.mp4")) // para testar com um video
    //capture.open(0);
    if (!capture.isOpened()) {
        cout << "Erro ao abrir a câmera!" << endl;
        return -1;
    }

    // --- Pegar dimensões da câmera ---
    Mat temp;
    capture >> temp;
    if (temp.empty()) {
        cout << "Não foi possível capturar frame da câmera!" << endl;
        return -1;
    }
    int camWidth = temp.cols;
    int camHeight = temp.rows;

    cascadeName = "haarcascade_frontalface_default.xml";
    if (!cascade.load(cascadeName)) {
        cout << "ERROR: Não consegui carregar cascade: " << cascadeName << endl;
        return -1;
    }

    // --- Carregar sprite ---
    orange = imread("naogrita.jpeg", IMREAD_COLOR);
    if (orange.empty()) {
        cout << "Erro ao carregar a laranja (orange.png)!" << endl;
        return -1;
    }

    namedWindow(wName, WINDOW_FULLSCREEN);
    Menu gameMenu(camWidth, camHeight, wName);
    gameMenu.setupMouseCallback();

    while (estado != SAIR) {
        if (estado == MENU) {
            gameMenu.showMainMenu();
            int key = waitKey(30);
            if (key == 27) estado = SAIR; // ESC para sair
        }
        else if (estado == JOGO) {
            Mat frame;
            capture >> frame;
            if (frame.empty()) break;

            render.detectAndDraw(frame, cascade, scale, tryflip, orange);

            char key = (char)waitKey(10);
            if (key == 27) estado = SAIR; // ESC fecha
            if (key == 'm') estado = MENU; // M retorna ao menu
        }
        else if (estado == OPTIONS) {
            gameMenu.showOptionsMenu();
            int key = waitKey(30);
            if (key == 27) estado = SAIR;
        }
        else if (estado == DESC) {
            gameMenu.showDescriptionMenu();
            int key = waitKey(30);
            if (key == 27) estado = SAIR;
        }
    }

    return 0;
}
