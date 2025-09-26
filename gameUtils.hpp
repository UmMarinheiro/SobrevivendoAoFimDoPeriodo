#ifndef GAMEUTILS_HPP
#define GAMEUTILS_HPP

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "gameInstance.hpp"
#include "Menu.hpp"
#include "globals.hpp"
#include <memory>

extern Estado estado;
extern std::string wName;
extern bool tryflip;
extern double scale;
extern bool jogadorCapturado;
extern Turno turno;


bool initializeCamera(cv::VideoCapture& capture);
bool initializeCascade(cv::CascadeClassifier& cascade, std::string& cascadeName);
void createPlayersDirectory();


void detectAndDraw(cv::Mat& frame, cv::CascadeClassifier& cascade, double scale, bool tryflip);
void handlePhotoTurn(cv::Mat& frame, cv::CascadeClassifier& cascade);
bool isRostoCentralizado(const cv::Rect& face, const cv::Size& frameSize, int tolerance = 50);
void aplicarOverlayGuia(cv::Mat& frame);


void salvarFotoJogador(const cv::Mat& originalFrame, const cv::Rect& face, int playerNumber);
int obterProximoNumeroJogador();


void handleMenuState(Menu& gameMenu);
void handleGameState(std::shared_ptr<GameInstance> game, cv::CascadeClassifier& cascade, cv::VideoCapture& capture);
void handleOptionsState(Menu& gameMenu);
void handleDescriptionState(Menu& gameMenu);

void drawImage(cv::Mat frame, cv::Mat img, int xPos, int yPos);

#endif 