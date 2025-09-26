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

Estado estado = MENU;

string cascadeName;
string wName = "Game";
bool tryflip = false;
double scale = 1;

CascadeClassifier cascade;
Mat img;
VideoCapture capture;

bool jogadorCapturado = false;
Turno turno = PHOTO;

int main()
{
    if (!initializeCamera(capture)) {
        return -1;
    }

    if (!initializeCascade(cascade, cascadeName)) {
        return -1;
    }

    createPlayersDirectory();

    int camWidth = 1920;
    int camHeight = 1080;
    
    namedWindow(wName, WINDOW_KEEPRATIO);
    Menu gameMenu(camWidth, camHeight, wName);
    gameMenu.setupMouseCallback();

    shared_ptr<GameInstance> game;

    while (estado != SAIR) {
        switch (estado) {
            case MENU:
                handleMenuState(gameMenu);
                break;
                
            case JOGO:
                game = make_shared<GameInstance>(wName, scale, tryflip, cascade, capture);
                estado = RODAR_JOGO;
                turno = PHOTO;
                break;
                
            case RODAR_JOGO:
                handleGameState(game, cascade, capture);
                break;
                
            case SAIR_JOGO:
                game.reset();
                estado = MENU;
                break;
                
            case OPTIONS:
                handleOptionsState(gameMenu);
                break;
                
            case DESC:
                handleDescriptionState(gameMenu);
                break;
        }
    }

    return 0;
}