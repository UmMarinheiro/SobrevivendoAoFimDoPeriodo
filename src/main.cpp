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
bool tryflip = true;
double scale = 1;

AudioManager audioManager;
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

    char key = (char)waitKey(30);

    int camWidth = 1920;
    int camHeight = 1080;
    
    namedWindow(wName, WINDOW_KEEPRATIO);
    Menu gameMenu(camWidth, camHeight, wName);
    gameMenu.setupMouseCallback();

    shared_ptr<GameInstance> game;

    if(estado == MENU) gameMenu.playBackgroundMusic();

    while (estado != SAIR) {

        switch (estado) {
            case MENU:
                handleMenuState(gameMenu);
                gameMenu.updateAnimations();
                break;
                
            case JOGO:
                game = make_shared<GameInstance>(wName, scale, tryflip, cascade, capture);
                estado = RODAR_JOGO;
                turno = PHOTO;
                //gameMenu.desbloquearItem("Mochila");
                //gameMenu.desbloquearItem("Marmita");
                break;
                
            case RODAR_JOGO:
                handleGameState(game, cascade, capture, gameMenu);
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
        switch(key){
            case 'j': audioManager.setSoundVolume( (audioManager.getSoundVolume() + 10)); break;
            case 'k': audioManager.setSoundVolume( (audioManager.getSoundVolume() - 10)); break;
            if (key == 'g') gameMenu.desbloquearItem("Marmita");
            if (key == 'g') gameMenu.desbloquearItem("Mochila");
            case 27: estado = SAIR; break;
        }
    }
    gameMenu.stopBackgroundMusic();
    return 0;
}
