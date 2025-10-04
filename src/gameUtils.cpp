#include "gameUtils.hpp"
#include "opencvUtils.hpp"
#include "sprite.hpp"
#include "colisor.hpp"
#include "Menu.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <random>
#include <fstream>

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

struct ItemData {
    string folderName;
    string imagePath;
    string description;
    int index = -1; 
};

ItemData getRandomItem() {
    ItemData item;
    vector<string> itemFolders;
    
    string itemsPath = "assets/items";
    
    if (!fs::exists(itemsPath)) {
        cout << "ERRO: Diretório assets/items não encontrado!" << endl;
        return item;
    }
    
    for (const auto& entry : fs::directory_iterator(itemsPath)) {
        if (entry.is_directory()) {
            itemFolders.push_back(entry.path().filename().string());
        }
    }
    
    if (itemFolders.empty()) {
        cout << "ERRO: Nenhuma pasta de item encontrada em assets/items!" << endl;
        return item;
    }
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, itemFolders.size() - 1);
    
    int selectedIndex = dis(gen);
    string selectedFolder = itemFolders[selectedIndex];
    item.folderName = selectedFolder;
    item.index = selectedIndex;
    
    string folderPath = itemsPath + "/" + selectedFolder;
    item.imagePath = folderPath + "/imagem.png";
    string descriptionPath = folderPath + "/descricao.txt";
    
    ifstream descFile(descriptionPath);
    if (descFile.is_open()) {
        getline(descFile, item.description);
        descFile.close();
    } else {
        cout << "AVISO: Arquivo descricao.txt não encontrado em " << folderPath << endl;
        item.description = "Sem descrição disponível";
    }
    
    if (!fs::exists(item.imagePath)) {
        cout << "AVISO: imagem.png não encontrada em " << folderPath << endl;
    }
    
    cout << "Item selecionado: " << selectedFolder << endl;
    
    return item;
}

void showItemScreen(const string& itemPath, const string& itemName, const string& itemDescription) {
    Mat blackScreen = Mat::zeros(480, 640, CV_8UC3);
    
    Mat itemImage = imread(itemPath, IMREAD_UNCHANGED);
    
    if (!itemImage.empty()) {
        int maxSize = 300;
        if (itemImage.cols > maxSize || itemImage.rows > maxSize) {
            double scale = min((double)maxSize / itemImage.cols, (double)maxSize / itemImage.rows);
            resize(itemImage, itemImage, Size(), scale, scale, INTER_LINEAR);
        }
        
        int itemX = (blackScreen.cols - itemImage.cols) / 2;
        int itemY = (blackScreen.rows - itemImage.rows) / 2;
        
        drawImage(blackScreen, itemImage, itemX, itemY);
        
        int fontFace = FONT_HERSHEY_SIMPLEX;
        double fontScale = 1.2;
        int thickness = 2;
        
        int baseline = 0;
        Size textSize = getTextSize(itemName, fontFace, fontScale, thickness, &baseline);
        
        int textX = (blackScreen.cols - textSize.width) / 2;
        int textY = itemY - 30;
        
        putText(blackScreen, itemName, Point(textX + 2, textY + 2), 
                fontFace, fontScale, Scalar(0, 0, 0), thickness + 1);
        putText(blackScreen, itemName, Point(textX, textY), 
                fontFace, fontScale, Scalar(255, 255, 255), thickness);
        
        double descFontScale = 0.7;
        int descThickness = 1;
        
        Size descSize = getTextSize(itemDescription, fontFace, descFontScale, descThickness, &baseline);
        int descX = (blackScreen.cols - descSize.width) / 2;
        int descY = itemY + itemImage.rows + 40;
        
        putText(blackScreen, itemDescription, Point(descX + 1, descY + 1), 
                fontFace, descFontScale, Scalar(0, 0, 0), descThickness + 1);
        putText(blackScreen, itemDescription, Point(descX, descY), 
                fontFace, descFontScale, Scalar(200, 200, 200), descThickness);
    } else {
        string errorMsg = "Item: " + itemName;
        putText(blackScreen, errorMsg, Point(50, 240), 
                FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);
    }
    
    imshow(wName, blackScreen);
    
    int64 startTime = getTickCount();
    double elapsedSeconds = 0;
    
    while (elapsedSeconds < 5.0) {
        int key = waitKey(100);
        
        if (key == 32 || key == ' ') {
            break;
        }
        
        int64 currentTime = getTickCount();
        elapsedSeconds = (currentTime - startTime) / getTickFrequency();
    }
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
            {
            ItemData randomItem = getRandomItem();
            if (!randomItem.folderName.empty()) {
                showItemScreen(randomItem.imagePath, ("Você recebeu o item: %s ", randomItem.folderName), randomItem.description);

                int playerNumber = obterProximoNumeroJogador() - 1;
                std::ofstream itensFile("assets/players/itens_recebidos.txt", std::ios::app);
                if (itensFile.is_open()) {
                    itensFile << "player " << playerNumber << " - " << randomItem.folderName << std::endl;
                    itensFile.close();
                }
            } else {
                cout << "ERRO: Não foi possível carregar um item!" << endl;
            }
            detectAndDraw(frame, cascade, scale, tryflip);
            game->startTurn(obterProximoNumeroJogador() - 1, randomItem.folderName); 

            turno = POSITION;
            break;
            }
            
        case POSITION:
            game->updateTurn();
            if (game->hasTurnEnded()) {
                game->endTurn();
                
                turno = PHOTO;
            }
            else if(game->hasGameEnded())
            {
                estado = SAIR_JOGO; 
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