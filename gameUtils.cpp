#include "gameUtils.hpp"
#include "opencvUtils.hpp"
#include "sprite.hpp"
#include "colisor.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;


bool initializeCamera(VideoCapture& capture) {
    if (!capture.open(0)) {
        cout << "Erro ao abrir a câmera!" << endl;
        return false;
    }

    Mat temp;
    capture >> temp;
    if (temp.empty()) {
        cout << "Não foi possível capturar frame da câmera!" << endl;
        return false;
    }
    return true;
}

bool initializeCascade(CascadeClassifier& cascade, string& cascadeName) {
    cascadeName = "config/haarcascade_frontalface_default.xml";
    if (!cascade.load(cascadeName)) {
        cout << "ERROR: Não consegui carregar cascade: " << cascadeName << endl;
        return false;
    }
    return true;
}

void createPlayersDirectory() {
    if (!fs::exists("assets/players")) {
        fs::create_directories("assets/players");
        cout << "Diretório assets/players criado!" << endl;
    }
}

void handleMenuState(Menu& gameMenu) {
    gameMenu.showMainMenu();
    int key = waitKey(30);
    if (key == 27) estado = SAIR; 
}

void handleGameState(shared_ptr<GameInstance> game, CascadeClassifier& cascade, VideoCapture& capture) {
    Mat frame;
    capture >> frame;
    if (frame.empty()) return;
    
    switch (turno) {
        case PHOTO:
            handlePhotoTurn(frame, cascade);
            if (jogadorCapturado) {
                jogadorCapturado = false;
                turno = ITEM;
            }
            break;
            
        case ITEM:
            detectAndDraw(frame, cascade, scale, tryflip);
            game->startTurn(obterProximoNumeroJogador() - 1);
            turno = POSITION;
            break;
            
        case POSITION:
            game->updateTurn();
            if (game->hasTurnEnded()) {
                game->endTurn();
                turno = PHOTO;
            }
            break;
    }

    char key = (char)waitKey(10);
    if (key == 27) estado = SAIR; 
    if (key == 'm') estado = SAIR_JOGO; 
    if (key == 'n') turno = PHOTO; 
}

void handleOptionsState(Menu& gameMenu) {
    gameMenu.showOptionsMenu();
    int key = waitKey(30);
    if (key == 27) estado = SAIR;
}

void handleDescriptionState(Menu& gameMenu) {
    gameMenu.showDescriptionMenu();
    int key = waitKey(30);
    if (key == 27) estado = SAIR;
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
            vector<Mat> channels;
            split(overlay, channels);

            Mat bgrOverlay, mask;
            merge(vector<Mat>{channels[0], channels[1], channels[2]}, bgrOverlay);
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

void detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip)
{
    Mat smallFrame = getFrame(frame, scale, tryflip);
    vector<Rect> faces = getFaces(frame, cascade);

    for (Rect r : faces) {        
        rectangle(smallFrame, Point(cvRound(r.x), cvRound(r.y)),
                Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))),
                Scalar(255,0,0), 3);
    }

    imshow(wName, smallFrame);
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

void drawImage(Mat frame, Mat img, int xPos, int yPos) {
    if (yPos + img.rows >= frame.rows || xPos + img.cols >= frame.cols || xPos <= 0 || yPos <= 0)
        return;

    Mat mask;
    vector<Mat> layers;

    split(img, layers);
    if (layers.size() == 4) {
        Mat rgb[3] = { layers[0], layers[1], layers[2] };
        mask = layers[3];
        merge(rgb, 3, img);
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols), mask);
    } else {
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols));
    }
}