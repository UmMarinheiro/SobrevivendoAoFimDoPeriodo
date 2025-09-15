#include "colisorMan.hpp"
#include "gameInstance.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "Menu.hpp" // Incluir o header da classe Menu
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

// --- Prototipos ---
void detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip);
void handlePhotoTurn(Mat& frame, CascadeClassifier& cascade);
bool isRostoCentralizado(const Rect& face, const Size& frameSize, int tolerance = 50);
void salvarFotoJogador(const Mat& originalFrame, const Rect& face, int playerNumber);
int obterProximoNumeroJogador();
void desenharGuiaCentral(Mat& frame);
void aplicarOverlayGuia(Mat& frame);

// --- Estado do jogo (definição da variável global) ---
Estado estado = MENU;

// --- Variáveis globais ---
string cascadeName;
string wName = "Game";
bool tryflip = false;
double scale = 1;

CascadeClassifier cascade;
Mat img;
VideoCapture capture;

bool jogadorCapturado = false;

void drawImage(Mat frame, Mat img, int xPos, int yPos) {
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

float axys = 0.0;

class Teste : public Transform
{
public:
    shared_ptr<Sprite> sprite_sptr;
    shared_ptr<Colisor> colisor_sptr;
    Teste(std::string asset, std::string id)
    {
        sprite_sptr = Sprite::createSprite(asset, (Transform*)this);
        colisor_sptr = Colisor::createColisor(id, (Transform*)this);
        auto [w,h] = sprite_sptr->getLocalSize();
        colisor_sptr->setLocalSize({w,h});
    }
};

int main()
{
    //--- Abrir câmera ---
    if(!capture.open(0)) // para testar com um video
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
    int camWidth = 1920;
    int camHeight = 1080;

    cascadeName = "config/haarcascade_frontalface_default.xml";
    if (!cascade.load(cascadeName)) {
        cout << "ERROR: Não consegui carregar cascade: " << cascadeName << endl;
        return -1;
    }

    if (!fs::exists("assets/players")) {
        fs::create_directories("assets/players");
        cout << "Diretório assets/players criado!" << endl;
    }

    namedWindow(wName, WINDOW_KEEPRATIO);
    Menu gameMenu(camWidth, camHeight, wName);
    gameMenu.setupMouseCallback();

    shared_ptr<GameInstance> game;

    while (estado != SAIR) {
        if (estado == MENU) {
            gameMenu.showMainMenu();
            int key = waitKey(30);
            if (key == 27) estado = SAIR; // ESC para sair
        }
        else if (estado == JOGO) {
            game = make_shared<GameInstance>(wName, scale, tryflip,
                cascade, capture);
            estado = RODAR_JOGO;
        }
        else if (estado == RODAR_JOGO){
            Mat frame;
            capture >> frame;
            if (frame.empty()) break;
            
            static Turno turno = PHOTO;
            
            if (turno == PHOTO) {
                handlePhotoTurn(frame, cascade);
                if (jogadorCapturado) {
                    jogadorCapturado = false;
                    turno = ITEM; 
                }
            }
            else if (turno == ITEM) {
                detectAndDraw(frame, cascade, scale, tryflip);
                game->startTurn(obterProximoNumeroJogador()-1);
                turno = POSITION; 
            }
            else if (turno == POSITION) {
                game->tick();
                if(game->hasTurnEnded()) turno = PHOTO;
            }

            char key = (char)waitKey(10);
            if (key == 27) estado = SAIR; // ESC fecha
            if (key == 'm') estado = SAIR_JOGO; // M retorna ao menu
            if (key == 'n') turno = PHOTO; // N força novo turno PHOTO (para testes)
        }
        else if (estado == SAIR_JOGO) {
            game.reset();
            estado = MENU;
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

void handlePhotoTurn(Mat& frame, CascadeClassifier& cascade) {
    Mat originalFrame = frame.clone();
    
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

    aplicarOverlayGuia(smallFrame);
    
    bool rostoCentralizado = false;
    Rect rostocentral;
    
    for (const Rect& face : faces) {
        if (isRostoCentralizado(face, smallFrame.size())) {
            rostoCentralizado = true;
            rostocentral = face;
            
            rectangle(smallFrame, face, Scalar(0, 255, 0), 3);
        } else {
            rectangle(smallFrame, face, Scalar(0, 0, 255), 3);
        }
    }
    
    if (rostoCentralizado) {
        int playerNumber = obterProximoNumeroJogador();
        
        Rect faceOriginal;
        faceOriginal.x = (int)(rostocentral.x / fx);
        faceOriginal.y = (int)(rostocentral.y / fx);
        faceOriginal.width = (int)(rostocentral.width / fx);
        faceOriginal.height = (int)(rostocentral.height / fx);
        
        if (tryflip) {
            faceOriginal.x = originalFrame.cols - faceOriginal.x - faceOriginal.width;
        }
        
        salvarFotoJogador(originalFrame, faceOriginal, playerNumber);
        jogadorCapturado = true;
        
        cout << "Jogador " << playerNumber << " capturado com sucesso!" << endl;
    }
    
    imshow(wName, smallFrame);
}

void aplicarOverlayGuia(Mat& frame) {
    Mat overlay = imread("assets/overlay.png", IMREAD_UNCHANGED);
    if (!overlay.empty()) {
        resize(overlay, overlay, frame.size());

        double alpha = 0.6;

        if (overlay.channels() == 4) {
            std::vector<Mat> channels;
            split(overlay, channels);

            Mat bgrOverlay, mask;
            merge(std::vector<Mat>{channels[0], channels[1], channels[2]}, bgrOverlay);
            mask = channels[3];

            Mat roi = frame(Rect(0, 0, bgrOverlay.cols, bgrOverlay.rows));

            Mat temp;
            addWeighted(bgrOverlay, alpha, roi, 1.0 - alpha, 0.0, temp);
            temp.copyTo(roi, mask);
        } else {
            Mat blended;
            addWeighted(overlay, alpha, frame, 1.0 - alpha, 0.0, blended);
            frame = blended;
        }
    }
}

bool isRostoCentralizado(const Rect& face, const Size& frameSize, int tolerance) {
    int centerX = frameSize.width / 2;
    int centerY = frameSize.height / 2;
    
    int faceCenterX = face.x + face.width / 2;
    int faceCenterY = face.y + face.height / 2;
    
    return (abs(faceCenterX - centerX) <= tolerance && 
            abs(faceCenterY - centerY) <= tolerance);
}

void salvarFotoJogador(const Mat& originalFrame, const Rect& face, int playerNumber) {
    int x = max(0, face.x);
    int y = max(0, face.y);
    int width = min(originalFrame.cols - x, face.width);
    int height = min(originalFrame.rows - y, face.height);
    
    int margin = 30;
    x = max(0, x - margin);
    y = max(0, y - margin);
    width = min(originalFrame.cols - x, width + 2 * margin);
    height = min(originalFrame.rows - y, height + 2 * margin);
    
    Rect safeFace(x, y, width, height);
    
    if (safeFace.width > 0 && safeFace.height > 0 && 
        safeFace.x + safeFace.width <= originalFrame.cols && 
        safeFace.y + safeFace.height <= originalFrame.rows) {
        
        Mat faceImage = originalFrame(safeFace);
        
        stringstream filename;
        filename << "assets/players/player" << playerNumber << ".png";
        
        if (imwrite(filename.str(), faceImage)) {
            cout << "Foto do jogador " << playerNumber << " salva em " << filename.str() << endl;
        } else {
            cout << "Erro ao salvar foto do jogador " << playerNumber << endl;
        }
    } else {
        cout << "Erro: Coordenadas do rosto inválidas para o jogador " << playerNumber << endl;
    }
}

int obterProximoNumeroJogador() {
    int maxPlayerNumber = 0;
    
    if (fs::exists("assets/players")) {
        for (const auto& entry : fs::directory_iterator("assets/players")) {
            if (entry.is_regular_file()) {
                string filename = entry.path().stem().string();
                if (filename.substr(0, 6) == "player") {
                    try {
                        int playerNum = stoi(filename.substr(6));
                        maxPlayerNumber = max(maxPlayerNumber, playerNum);
                    } catch (const exception& e) {
                    }
                }
            }
        }
    }
    
    return maxPlayerNumber + 1;
}

// Detecta rostos mas não desenha nada em cima deles
void detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip)
{
    Mat smallFrame = getFrame(frame, scale, tryflip);
    vector<Rect> faces = getFaces(frame, cascade);

    for (Rect r : faces) {        
        rectangle( smallFrame, Point(cvRound(r.x), cvRound(r.y)),
                Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))),
                Scalar(255,0,0), 3);
    }

    imshow(wName, smallFrame);
}