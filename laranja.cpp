#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

// --- Prototipos ---
void detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip, Mat& orange);
void overlayImage(Mat &background, Mat &foreground, Point location);
void mouseCallback(int event, int x, int y, int, void* userdata);

// --- Estados do jogo ---
enum Estado { MENU, JOGO, SAIR, OPTIONS, DESC }; //TODO -pause é estado a parte ou parte de jogo
Estado estado;
// --- Variáveis globais ---
string cascadeName;
string wName = "Game";
Rect botaoJogar, botaoSair, botaoOpcoes, botaoDesc, botaoLeave;
bool tryflip = true;
double scale = 1;

CascadeClassifier cascade;
Mat orange;
VideoCapture capture;

int main()
{
    //--- Abrir câmera ---
    //if(!capture.open("video.mp4")) // para testar com um video
    capture.open(0);
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

    // --- Inicializar Haarcascade ---
    cascadeName = "haarcascade_frontalface_default.xml";
    if (!cascade.load(cascadeName)) {
        cout << "ERROR: Não consegui carregar cascade: " << cascadeName << endl;
        return -1;
    }

    // --- Carregar sprite ---
    orange = imread("naogrita.jpeg", IMREAD_UNCHANGED);
    if (orange.empty()) {
        cout << "Erro ao carregar a laranja (orange.png)!" << endl;
        return -1;
    }

    // --- Criar janela única ---
    namedWindow(wName, WINDOW_FULLSCREEN);
    setMouseCallback(wName, mouseCallback, NULL);

    // --- Laço principal ---
    while (estado != SAIR) {
        if (estado == MENU) {
            // desenha menu do tamanho da camera
            Mat menu(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));

            // Título
            putText(menu, "Sobrevivendo Ao Fim Do Periodo",
                    Point(camWidth * 0.25 - 35, camHeight * 0.25),
                    FONT_HERSHEY_TRIPLEX , 2.0,
                    Scalar(255, 255, 255), 3);

            botaoJogar = Rect(camWidth * 0.5 - 90, camHeight * 0.5 - 50, 240, 60);
            rectangle(menu, botaoJogar, Scalar(255, 255, 255), 2); //Cor da borda
            putText(menu, "Play",
                    Point(botaoJogar.x + 85, botaoJogar.y + 40),
                    FONT_HERSHEY_TRIPLEX , 1.0,
                    Scalar(255, 255, 255), 2); //Cor do nome

            botaoOpcoes = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 50, 240, 60);
            rectangle(menu, botaoOpcoes, Scalar(255, 255, 255), 2);
            putText(menu, "Options",
                    Point(botaoOpcoes.x + 55, botaoOpcoes.y + 40),
                    FONT_HERSHEY_TRIPLEX , 1.0,
                    Scalar(255, 255, 255), 2);
            botaoDesc = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 150, 240, 60);
            rectangle(menu, botaoDesc, Scalar(255, 255, 255), 2);
            putText(menu, "Description",
                    Point(botaoDesc.x + 20, botaoDesc.y + 40),
                    FONT_HERSHEY_TRIPLEX , 1.0,
                    Scalar(255, 255, 255), 2);
                
            botaoSair = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
            rectangle(menu, botaoSair, Scalar(255, 255, 255), 2);
            putText(menu, "Exit",
                    Point(botaoSair.x + 75, botaoSair.y + 40),
                    FONT_HERSHEY_TRIPLEX , 1.0,
                    Scalar(255, 255, 255), 2);

            imshow(wName, menu);
            int key = waitKey(30);
            if (key == 27) estado = SAIR; // ESC para sair
        }
        else if (estado == JOGO) {
            Mat frame;
            capture >> frame;
            if (frame.empty()) break;

            detectAndDraw(frame, cascade, scale, tryflip, orange);

            char key = (char)waitKey(10);
            if (key == 27) estado = SAIR; // ESC fecha
            if (key == 'm') estado = MENU; // M retorna ao menu
        }
        else if (estado == OPTIONS){
            Mat options(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));

            putText(options, "Options",
                Point(camWidth * 0.25 + 200, camHeight * 0.25),
                FONT_HERSHEY_TRIPLEX , 2.0,
                Scalar(255, 255, 255), 3);

            botaoLeave = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
            rectangle(options, botaoLeave, Scalar(255, 255, 255), 2);
            putText(options, "Leave",
            Point(botaoLeave.x + 70, botaoLeave.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 3);

            imshow(wName, options);
            int key = waitKey(30);
            if (key == 27) estado = SAIR;
        }
        else if (estado == DESC){
            Mat desc(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));

            putText(desc, "Description",
                Point(camWidth * 0.25 + 200, camHeight * 0.25),
                FONT_HERSHEY_TRIPLEX , 1.0,
                Scalar(255, 255, 255), 3);

            botaoLeave = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
            rectangle(desc, botaoLeave, Scalar(255, 255, 255), 2);
            putText(desc, "Leave",
            Point(botaoLeave.x + 70, botaoLeave.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 3);

            imshow(wName, desc);
            int key = waitKey(30);
            if (key == 27) estado = SAIR;
        }
    }

    return 0;
}

// --- Funções ---

// Detecta rostos e desenha laranja em cima
void detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip, Mat& orange)
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

    // percorre as faces encontradas
    for (Rect r : faces) {
        Mat orange_resized;
        resize(orange, orange_resized, Size(r.width, r.height));
        overlayImage(smallFrame, orange_resized, Point(r.x, r.y));
    }

    imshow(wName, smallFrame);
}

// Sobreposição de imagem com alfa (PNG)
void overlayImage(Mat &background, Mat &foreground, Point location) {
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

// Callback do mouse (para o menu)
void mouseCallback(int event, int x, int y, int, void*) {
    if (estado == MENU && event == EVENT_LBUTTONDOWN) {
        if (botaoJogar.contains(Point(x, y))) {
            cout << "Jogar clicado!" << endl;
            estado = JOGO;
        }
        if (botaoSair.contains(Point(x, y))) {
            cout << "Sair clicado!" << endl;
            estado = SAIR;
        }
        if (botaoOpcoes.contains(Point(x, y))) {
            cout << "Options clicado!" << endl;
            estado = OPTIONS;
        }
        if(botaoDesc.contains(Point(x, y))){
            cout << "Description clicado!" << endl;
            estado = DESC;
        }
    }
    if(estado == OPTIONS && event == EVENT_LBUTTONDOWN){
        if (botaoLeave.contains(Point(x, y))){
        cout << "Leave options clicado!" << endl;
        estado = MENU;
        }
    }
    if(estado == DESC && event == EVENT_LBUTTONDOWN){
        if(botaoLeave.contains(Point(x, y))){
        cout << "Leave desc clicado!" << endl;
        estado = MENU;
        }
    }
}