#include "Menu.hpp"
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace std;

Menu::Menu(int width, int height, const string& wName) 
    : camWidth(width), camHeight(height), windowName(wName),
      animBotaoJogar(Rect(1600, 600, 240, 60), "Play"),     
      animBotaoOpcoes(Rect(1600, 700, 240, 60), "Options"),
      animBotaoDesc(Rect(1600, 800, 240, 60), "Description"),
      animBotaoSair(Rect(1600, 900, 240, 60), "Exit"),
      animBotaoLeave(Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60), "Leave"),
      wasJogarHovered(false), wasOpcoesHovered(false), wasDescHovered(false), 
      wasSairHovered(false), wasLeaveHovered(false) {
    loadMenuImages();
    initializeButtonAnimations();
}

Menu::~Menu(){}

AudioManager audio;

void Menu::playSound(const string& filePath) {
    string command;

    command = "aplay \"" + filePath + "\" &";

    int result = system(command.c_str());
    if (result != 0) {
        cout << "Erro ao reproduzir audio: " << filePath << endl;
    }
}

void Menu::playHoverSound() {
    playSound("assets/audio/click.wav");
}

void Menu::playClickSound() {
    playSound("assets/audio/click.wav");
}

void Menu::playBackgroundMusic() {
    system("aplay assets/audio/background.wav &");
}

void Menu::stopBackgroundMusic() {
    system("pkill aplay 2>/dev/null");
}

void Menu::loadMenuImages() {
    Mat bgMain = imread("assets/images/bck.png", IMREAD_COLOR);
    if (!bgMain.empty()) {
        menuImages["main_bg"] = bgMain;
    }
    
    Mat bgOptions = imread("assets/images/bg_options.png", IMREAD_COLOR);
    if (!bgOptions.empty()) {
        menuImages["options_bg"] = bgOptions;
    }
    
    Mat logo = imread("assets/images/logo.png", IMREAD_UNCHANGED);
    if (!logo.empty()) {
        menuImages["logo"] = logo;
    }
    
    Mat descriptionIcon = imread("assets/images/desc_icon.png", IMREAD_UNCHANGED);
    if (!descriptionIcon.empty()){
        menuImages["description_icon"] = descriptionIcon;
    }

    Mat playIcon = imread("assets/images/orange.png", IMREAD_UNCHANGED);
    if (!playIcon.empty()) {
        menuImages["play_icon"] = playIcon;
    }
    
    Mat optionsIcon = imread("assets/images/orange.png", IMREAD_UNCHANGED);
    if (!optionsIcon.empty()) {
        menuImages["options_icon"] = optionsIcon;
    }
    
    Mat exitIcon = imread("assets/images/orange.png", IMREAD_UNCHANGED);
    if (!exitIcon.empty()) {
        menuImages["exit_icon"] = exitIcon;
    }
    
    Mat descIcon = imread("assets/images/orange.png", IMREAD_UNCHANGED);
    if (!descIcon.empty()) {
        menuImages["desc_icon"] = descIcon;
    }
    
    Mat quadradoIcon = imread("assets/images/oscar.png", IMREAD_UNCHANGED);
    if(!quadradoIcon.empty()){
        menuImages["quadrado"] = quadradoIcon;
    }
    
    Mat quadrado1Icon = imread("assets/images/orange.png", IMREAD_UNCHANGED);
    if(!quadrado1Icon.empty()){
        menuImages["quadrado1"] = quadrado1Icon;
    }
    
    Mat quadrado2Icon = imread("assets/images/arrascaeta.jpeg", IMREAD_UNCHANGED);
    if(!quadrado2Icon.empty()){
        menuImages["quadrado2"] = quadrado2Icon;
    }
    Mat volumePlus = imread("assets/images/plus.png", IMREAD_UNCHANGED);
    if(!volumePlus.empty()){
        menuImages["volumePlus"] = volumePlus;
    }
    Mat volumeMinus = imread("assets/images/minus.png", IMREAD_UNCHANGED);
    if(!volumeMinus.empty()){
        menuImages["volumeMinus"] = volumeMinus;
    }
}

void Menu::initializeButtonAnimations() {
    animBotaoJogar.setStaticFrame(Scalar(255, 100, 100));
    animBotaoJogar.hoverAnimation.generatePulseFrames(240, 60, Scalar(200, 100, 100), 10);
    animBotaoJogar.hoverAnimation.setFrameDuration(0.06);
    
    animBotaoOpcoes.setStaticFrame(Scalar(100, 100, 100));
    animBotaoOpcoes.hoverAnimation.generatePulseFrames(240, 60, Scalar(150, 150, 150), 10);
    animBotaoOpcoes.hoverAnimation.setFrameDuration(0.06);
    
    animBotaoDesc.setStaticFrame(Scalar(100, 100, 100));
    animBotaoDesc.hoverAnimation.generatePulseFrames(240, 60, Scalar(150, 150, 150), 10);
    animBotaoDesc.hoverAnimation.setFrameDuration(0.06);
    
    animBotaoSair.setStaticFrame(Scalar(100, 100, 100));
    animBotaoSair.hoverAnimation.generatePulseFrames(240, 60, Scalar(150, 150, 150), 10);
    animBotaoSair.hoverAnimation.setFrameDuration(0.06);
    
    animBotaoLeave.setStaticFrame(Scalar(100, 100, 100));
    animBotaoLeave.hoverAnimation.generatePulseFrames(240, 60, Scalar(150, 150, 150), 10);
    animBotaoLeave.hoverAnimation.setFrameDuration(0.06);
}

void Menu::updateAnimations() {
    animBotaoJogar.updateAnimation();
    animBotaoOpcoes.updateAnimation();
    animBotaoDesc.updateAnimation();
    animBotaoSair.updateAnimation();
    animBotaoLeave.updateAnimation();
}

void Menu::drawImageOnMenu(Mat& menu, const Mat& image, Point position, Size targetSize) {
    if (image.empty()) return;
    
    Mat imgToRender = image.clone();
    
    if (targetSize.width > 0 && targetSize.height > 0) {
        resize(imgToRender, imgToRender, targetSize);
    }
    
    if (position.x + imgToRender.cols > menu.cols || 
        position.y + imgToRender.rows > menu.rows ||
        position.x < 0 || position.y < 0) {
        return;
    }
    
    if (imgToRender.channels() == 4) {
        Mat mask;
        vector<Mat> layers;
        
        split(imgToRender, layers);
        Mat rgb[3] = { layers[0], layers[1], layers[2] };
        mask = layers[3];
        merge(rgb, 3, imgToRender);
        
        imgToRender.copyTo(menu(Rect(position.x, position.y, imgToRender.cols, imgToRender.rows)), mask);
    } else {
        imgToRender.copyTo(menu(Rect(position.x, position.y, imgToRender.cols, imgToRender.rows)));
    }
}

void Menu::drawMainMenu(Mat& menu) {
    if (menuImages.find("main_bg") != menuImages.end()) {
        Mat background = menuImages["main_bg"];
        resize(background, background, Size(camWidth, camHeight));
        background.copyTo(menu);
    }
    
    if (menuImages.find("logo") != menuImages.end()) {
        Mat logo = menuImages["logo"];
        Point logoPos(camWidth/2 - 150, 50);
        drawImageOnMenu(menu, logo, logoPos, Size(300, 100));
    } else {
        putText(menu, "Sobrevivendo Ao Fim Do Periodo",
                Point(camWidth * 0.25 - 35, camHeight * 0.25),
                FONT_HERSHEY_TRIPLEX, 2.0,
                Scalar(255, 255, 255), 3);
    }

    botaoJogar = animBotaoJogar.rect;
    botaoOpcoes = animBotaoOpcoes.rect;
    botaoDesc = animBotaoDesc.rect;
    botaoSair = animBotaoSair.rect;

    Mat jogarSprite = animBotaoJogar.getCurrentSprite();
    jogarSprite.copyTo(menu(botaoJogar));
    
    Mat opcoesSprite = animBotaoOpcoes.getCurrentSprite();
    opcoesSprite.copyTo(menu(botaoOpcoes));
    
    Mat descSprite = animBotaoDesc.getCurrentSprite();
    descSprite.copyTo(menu(botaoDesc));
    
    Mat sairSprite = animBotaoSair.getCurrentSprite();
    sairSprite.copyTo(menu(botaoSair));

    putText(menu, "Play", Point(botaoJogar.x + 85, botaoJogar.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 255, 255), 2);

    putText(menu, "Options", Point(botaoOpcoes.x + 60, botaoOpcoes.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 255, 255), 2);

    putText(menu, "Description", Point(botaoDesc.x + 20, botaoDesc.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 255, 255), 2);

    putText(menu, "Exit", Point(botaoSair.x + 75, botaoSair.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 255, 255), 2);
}

void Menu::drawOptionsMenu(Mat& options) {
    if (menuImages.find("options_bg") != menuImages.end()) {
        Mat background = menuImages["options_bg"];
        resize(background, background, Size(camWidth, camHeight));
        background.copyTo(options);
    }
    
    putText(options, "Options", Point(850, 300),
            FONT_HERSHEY_TRIPLEX, 2.0, Scalar(255, 255, 255), 3);

        volumep = Rect(450, 450, 50, 50);
        volumem = Rect(200, 450, 50, 50);
        
    if (menuImages.find("volumePlus") != menuImages.end()) {
        Mat volumePlus = menuImages["volumePlus"];
        Point volumePlusPos(450, 500);
        drawImageOnMenu(options, volumePlus, volumePlusPos, Size(300, 100));
    } else {
        putText(options, "+",
                Point(450, 500),
                FONT_HERSHEY_TRIPLEX, 2.0,
                Scalar(255, 255, 255), 3);
    }
    if (menuImages.find("volumeMinus") != menuImages.end()) {
        Mat volumeMinus = menuImages["volumeMinus"];
        Point volumeMinusPos(200, 500);
        drawImageOnMenu(options, volumeMinus, volumeMinusPos, Size(300, 100));
    } else {
        putText(options, "-",
                Point(200, 500),
                FONT_HERSHEY_TRIPLEX, 2.0,
                Scalar(255, 255, 255), 3);
    }

    animBotaoLeave.rect = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
    botaoLeave = animBotaoLeave.rect;

    Mat leaveSprite = animBotaoLeave.getCurrentSprite();
    leaveSprite.copyTo(options(botaoLeave));
    
    putText(options, "Leave", Point(botaoLeave.x + 70, botaoLeave.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 255, 255), 3);
    
}

void Menu::drawDescriptionMenu(Mat& desc) {
    if (menuImages.find("description_icon") != menuImages.end()) {
        Point iconPos(camWidth * 0.25 + 200, camHeight * 0.1);
        drawImageOnMenu(desc, menuImages["description_icon"], iconPos, Size(500, 200));
    } else {
        putText(desc, "Description", Point(camWidth * 0.25 + 200, camHeight * 0.25),
                FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 255, 255), 3);
    }

    if (q == true){
        putText(desc, "Desc1", Point(camWidth * 0.68 - 1, camHeight * 0.25),
                FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 255, 255), 2);
    }
    if(q1 == true){
        putText(desc, "Desc2", Point(camWidth * 0.68 - 1, camHeight * 0.25),
                FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 0, 0), 2);
    }

    linhaVertical = Rect(camWidth * 0.65 - 1, camHeight * 0.2, 2, camHeight * 0.5);
    rectangle(desc, linhaVertical, Scalar(255, 255, 255), FILLED);

    quadrado = Rect(300, 350, 100, 100);
    rectangle(desc, quadrado, Scalar(255, 255, 255), 2);
    if(menuImages.find("quadrado") != menuImages.end()){    
        Point iconPos(quadrado.x + 10, quadrado.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado"], iconPos, Size(80, 80));
    }

    animBotaoLeave.rect = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
    botaoLeave = animBotaoLeave.rect;

    Mat leaveSprite = animBotaoLeave.getCurrentSprite();
    leaveSprite.copyTo(desc(botaoLeave));
    
    putText(desc, "Leave", Point(botaoLeave.x + 70, botaoLeave.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(255, 255, 255), 3);
}

void Menu::showMainMenu() {
    Mat menu(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));
    drawMainMenu(menu);
    imshow(windowName, menu);
}

void Menu::showOptionsMenu() {
    Mat options(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));
    drawOptionsMenu(options);
    imshow(windowName, options);
}

void Menu::showDescriptionMenu() {
    Mat desc(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));
    drawDescriptionMenu(desc);
    imshow(windowName, desc);
}

void Menu::mouseCallbackStatic(int event, int x, int y, int flags, void* userdata) {
    Menu* menu = static_cast<Menu*>(userdata);
    if (menu) {
        menu->mouseCallback(event, x, y, flags);
    }
}

void Menu::mouseCallback(int event, int x, int y, int flags) {
    bool previousJogarHovered = animBotaoJogar.isHovered;
    bool previousOpcoesHovered = animBotaoOpcoes.isHovered;
    bool previousDescHovered = animBotaoDesc.isHovered;
    bool previousSairHovered = animBotaoSair.isHovered;
    bool previousLeaveHovered = animBotaoLeave.isHovered;
    
    animBotaoJogar.isHovered = botaoJogar.contains(Point(x, y));
    animBotaoOpcoes.isHovered = botaoOpcoes.contains(Point(x, y));
    animBotaoDesc.isHovered = botaoDesc.contains(Point(x, y));
    animBotaoSair.isHovered = botaoSair.contains(Point(x, y));
    animBotaoLeave.isHovered = botaoLeave.contains(Point(x, y));

    if (!previousJogarHovered && animBotaoJogar.isHovered && estado == MENU) {
        playHoverSound();
    }
    if (!previousOpcoesHovered && animBotaoOpcoes.isHovered && estado == MENU) {
        playHoverSound();
    }
    if (!previousDescHovered && animBotaoDesc.isHovered && estado == MENU) {
        playHoverSound();
    }
    if (!previousSairHovered && animBotaoSair.isHovered && estado == MENU) {
        playHoverSound();
    }
    if (!previousLeaveHovered && animBotaoLeave.isHovered && (estado == OPTIONS || estado == DESC)) {
        playHoverSound();
    }

    if (estado == MENU && event == EVENT_LBUTTONDOWN) {
        if (botaoJogar.contains(Point(x, y))) {
            playClickSound();
            cout << "Jogar clicado!" << endl;
            estado = JOGO;
        }
        if (botaoSair.contains(Point(x, y))) {
            playClickSound();
            cout << "Sair clicado!" << endl;
            estado = SAIR;
        }
        if (botaoOpcoes.contains(Point(x, y))) {
            playClickSound();
            cout << "Options clicado!" << endl;
            estado = OPTIONS;
        }
        if (botaoDesc.contains(Point(x, y))) {
            playClickSound();
            cout << "Description clicado!" << endl;
            estado = DESC;
        }
    }
    
    if (estado == OPTIONS && event == EVENT_LBUTTONDOWN) {
        if (botaoLeave.contains(Point(x, y))) {
            playClickSound();
            estado = MENU;
        }
        if (volumep.contains(Point(x, y))) {
            playClickSound();
            audio.setSoundVolume(min(audio.getSoundVolume() + 10, 100));
        }
        if(volumem.contains(Point(x, y))){
            playClickSound();
            audio.setSoundVolume(max(audio.getSoundVolume() - 10, 0));
        }
    }
    
    if (estado == DESC && event == EVENT_LBUTTONDOWN) {
        if (botaoLeave.contains(Point(x, y))) {
            playClickSound();
            cout << "Leave desc clicado!" << endl;
            q = false; q1 = false; q2 = false; q3 = false;
            q4 = false; q5 = false; q6 = false; q7 = false; q8 = false;
            estado = MENU;
        }
    }
}

void Menu::setupMouseCallback() {
    setMouseCallback(windowName, mouseCallbackStatic, this);
}