#include "Menu.hpp"
#include <iostream>

Menu::Menu(int width, int height, const std::string& wName) 
    : camWidth(width), camHeight(height), windowName(wName) {
    loadMenuImages();
}

Menu::~Menu(){}

// Carrega todas as imagens dos menus
void Menu::loadMenuImages() {
    // Carrega imagens de fundo para diferentes menus
    Mat bgMain = imread("assets/naogrita.jpeg", IMREAD_COLOR);
    if (!bgMain.empty()) {
        menuImages["main_bg"] = bgMain;
    }
    
    Mat bgOptions = imread("assets/options_bg.png", IMREAD_COLOR);
    if (!bgOptions.empty()) {
        menuImages["options_bg"] = bgOptions;
    }
    
    // Carrega ícones/logos
    Mat logo = imread("assets/logo.png", IMREAD_UNCHANGED);
    if (!logo.empty()) {
        menuImages["logo"] = logo;
    }
    Mat descriptionIcon = imread("assets/desc_icon.png", IMREAD_UNCHANGED);
    if (!descriptionIcon.empty()){
        menuImages["description_icon"] = descriptionIcon;
    }

    // Carrega ícones dos botões
    Mat playIcon = imread("assets/orange.png", IMREAD_UNCHANGED);
    if (!playIcon.empty()) {
        menuImages["play_icon"] = playIcon;
    }
    
    Mat optionsIcon = imread("assets/orange.png", IMREAD_UNCHANGED);
    if (!optionsIcon.empty()) {
        menuImages["options_icon"] = optionsIcon;
    }
    
    Mat exitIcon = imread("assets/orange.png", IMREAD_UNCHANGED);
    if (!exitIcon.empty()) {
        menuImages["exit_icon"] = exitIcon;
    }
    
    Mat descIcon = imread("assets/orange.png", IMREAD_UNCHANGED);
    if (!descIcon.empty()) {
        menuImages["desc_icon"] = descIcon;
    }
    //Quadrados do description
    Mat quadradoIcon = imread("assets/oscar.png", IMREAD_UNCHANGED);
    if(!quadradoIcon.empty()){
        menuImages["quadrado"] = quadradoIcon;
    }
    Mat quadrado1Icon = imread("assets/orange.png", IMREAD_UNCHANGED);
    if(!quadrado1Icon.empty()){
        menuImages["quadrado1"] = quadrado1Icon;
    }
    Mat quadrado2Icon = imread("assets/quadrado2.png", IMREAD_UNCHANGED);
    if(!quadrado2Icon.empty()){
        menuImages["quadrado2"] = quadrado2Icon;
    }
    Mat quadrado3Icon = imread("assets/quadrado3.png", IMREAD_UNCHANGED);
    if(!quadrado3Icon.empty()){
        menuImages["quadrado3"] = quadrado3Icon;
    }
    Mat quadrado4Icon = imread("assets/quadrado4.png", IMREAD_UNCHANGED);
    if(!quadrado4Icon.empty()){
        menuImages["quadrado4"] = quadrado4Icon;
    }
    Mat quadrado5Icon = imread("assets/quadrado5.png", IMREAD_UNCHANGED);
    if(!quadrado5Icon.empty()){
        menuImages["quadrado5"] = quadrado5Icon;
    }
    Mat quadrado6Icon = imread("assets/quadrado6.png", IMREAD_UNCHANGED);
    if(!quadrado6Icon.empty()){
        menuImages["quadrado6"] = quadrado6Icon;
    }
    Mat quadrado7Icon = imread("assets/quadrado7.png", IMREAD_UNCHANGED);
    if(!quadrado7Icon.empty()){
        menuImages["quadrado7"] = quadrado7Icon;
    }
    Mat quadrado8Icon = imread("assets/quadrado8.png", IMREAD_UNCHANGED);
    if(!quadrado8Icon.empty()){
        menuImages["quadrado8"] = quadrado8Icon;
    }
}

// Método para desenhar imagem com suporte a transparência e redimensionamento
void Menu::drawImageOnMenu(Mat& menu, const Mat& image, Point position, Size targetSize) {
    if (image.empty()) return;
    
    Mat imgToRender = image.clone();
    
    // Redimensiona se um tamanho específico foi solicitado
    if (targetSize.width > 0 && targetSize.height > 0) {
        resize(imgToRender, imgToRender, targetSize);
    }
    
    // Verifica se a imagem cabe na tela
    if (position.x + imgToRender.cols > menu.cols || 
        position.y + imgToRender.rows > menu.rows ||
        position.x < 0 || position.y < 0) {
        return;
    }
    
    // Se a imagem tem canal alfa (transparência)
    if (imgToRender.channels() == 4) {
        Mat mask;
        std::vector<Mat> layers;
        
        split(imgToRender, layers);
        Mat rgb[3] = { layers[0], layers[1], layers[2] };
        mask = layers[3];
        merge(rgb, 3, imgToRender);
        
        imgToRender.copyTo(menu.rowRange(position.y, position.y + imgToRender.rows)
                              .colRange(position.x, position.x + imgToRender.cols), mask);
    } else {
        // Imagem sem transparência
        imgToRender.copyTo(menu.rowRange(position.y, position.y + imgToRender.rows)
                              .colRange(position.x, position.x + imgToRender.cols));
    }
}

// Método para desenhar o menu principal
void Menu::drawMainMenu(Mat& menu) {
    // Desenha imagem de fundo se existir
    if (menuImages.find("main_bg") != menuImages.end()) {
        Mat background = menuImages["main_bg"];
        resize(background, background, Size(camWidth, camHeight));
        background.copyTo(menu);
    }
    
    // Desenha logo no topo se existir
    if (menuImages.find("logo") != menuImages.end()) {
        Mat logo = menuImages["logo"];
        Point logoPos(camWidth/2 - 150, 50); // Centraliza horizontalmente
        drawImageOnMenu(menu, logo, logoPos, Size(300, 100));
    } else {
        // Título texto se não houver logo
        putText(menu, "Sobrevivendo Ao Fim Do Periodo",
                Point(camWidth * 0.25 - 35, camHeight * 0.25),
                FONT_HERSHEY_TRIPLEX, 2.0,
                Scalar(255, 255, 255), 3);
    }

    // Botão Jogar
    botaoJogar = Rect(camWidth * 0.5 - 120, camHeight * 0.5 - 50, 240, 60);
    rectangle(menu, botaoJogar, Scalar(255, 255, 255), 2);
    
    // Ícone do Play se existir
    if(play && menuImages.find("play_icon") != menuImages.end()){    
        Point iconPos(botaoJogar.x + 10, botaoJogar.y + 10);
        drawImageOnMenu(menu, menuImages["play_icon"], iconPos, Size(40, 40));
        putText(menu, "Play",
                Point(botaoJogar.x + 85, botaoJogar.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
        } else {
        putText(menu, "Play",
                Point(botaoJogar.x + 85, botaoJogar.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    }

    // Botão Opções
    botaoOpcoes = Rect(camWidth * 0.5 - 120, camHeight * 0.5 + 50, 240, 60);
    rectangle(menu, botaoOpcoes, Scalar(255, 255, 255), 2);
    
    if(opcoes && menuImages.find("options_icon") != menuImages.end()){
        Point iconPos(botaoOpcoes.x + 10, botaoOpcoes.y + 10);
        drawImageOnMenu(menu, menuImages["options_icon"], iconPos, Size(40, 40));
        putText(menu, "Options",
                Point(botaoOpcoes.x + 60, botaoOpcoes.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    } else {
        putText(menu, "Options",
                Point(botaoOpcoes.x + 60, botaoOpcoes.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    }

    // Botão Description
    botaoDesc = Rect(camWidth * 0.5 - 120, camHeight * 0.5 + 150, 240, 60);
    rectangle(menu, botaoDesc, Scalar(255, 255, 255), 2);
    
    if(description && menuImages.find("desc_icon") != menuImages.end()){
        Point iconPos(botaoDesc.x + 10, botaoDesc.y + 10);
        drawImageOnMenu(menu, menuImages["desc_icon"], iconPos, Size(40, 40));
        putText(menu, "Description",
                Point(botaoDesc.x + 20, botaoDesc.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    } else {
        putText(menu, "Description",
                Point(botaoDesc.x + 20, botaoDesc.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    }

    // Botão Sair
    botaoSair = Rect(camWidth * 0.5 - 120, camHeight * 0.5 + 250, 240, 60);
    rectangle(menu, botaoSair, Scalar(255, 255, 255), 2);
    
    if(sair && menuImages.find("exit_icon") != menuImages.end()){
        Point iconPos(botaoSair.x + 10, botaoSair.y + 10);
        drawImageOnMenu(menu, menuImages["exit_icon"], iconPos, Size(40, 40));
        putText(menu, "Exit",
                Point(botaoSair.x + 75, botaoSair.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    } else {
        putText(menu, "Exit",
                Point(botaoSair.x + 75, botaoSair.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    }
}

// Método para desenhar o menu de opções
void Menu::drawOptionsMenu(Mat& options) {
    // Fundo específico para opções se existir
    if (menuImages.find("options_bg") != menuImages.end()) {
        Mat background = menuImages["options_bg"];
        resize(background, background, Size(camWidth, camHeight));
        background.copyTo(options);
    }
    
    putText(options, "Options",
            Point(camWidth * 0.25 + 200, camHeight * 0.25),
            FONT_HERSHEY_TRIPLEX, 2.0,
            Scalar(255, 255, 255), 3);

    botaoLeave = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
    rectangle(options, botaoLeave, Scalar(255, 255, 255), 2);
    putText(options, "Leave",
            Point(botaoLeave.x + 70, botaoLeave.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 3);
}

// Método para desenhar o menu de descrição
void Menu::drawDescriptionMenu(Mat& desc) {
    if (menuImages.find("description_icon") != menuImages.end()) {
        Point iconPos(camWidth * 0.25 + 200, camHeight * 0.1);
        drawImageOnMenu(desc, menuImages["description_icon"], iconPos, Size(500, 200));
    }else{
    putText(desc, "Description",
            Point(camWidth * 0.25 + 200, camHeight * 0.25),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 3);

    }
    if (q == true){
        putText(desc, "Desc1",
            Point(camWidth * 0.68 - 1, camHeight * 0.25),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 2);
    }
    if(q1 == true){
        putText(desc, "Desc2",
            Point(camWidth * 0.68 - 1, camHeight * 0.25),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 0, 0), 2);
    }

    linhaVertical = Rect(camWidth * 0.65 - 1, camHeight * 0.2, 2, camHeight * 0.5);
    rectangle(desc, linhaVertical, Scalar(255, 255, 255), FILLED);

    quadrado = Rect(300, 350, 100, 100);
    rectangle(desc, quadrado, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado") != menuImages.end()){    
        Point iconPos(quadrado.x + 10, quadrado.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado"], iconPos, Size(80, 80));
    }

    quadrado1 = Rect(300, 500, 100, 100);
    rectangle(desc, quadrado1, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado1") != menuImages.end()){    
        Point iconPos(quadrado1.x + 10, quadrado1.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado1"], iconPos, Size(80, 80));
    }

    quadrado2 = Rect(300, 650, 100, 100);
    rectangle(desc, quadrado2, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado2") != menuImages.end()){    
        Point iconPos(quadrado2.x + 10, quadrado2.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado2"], iconPos, Size(80, 80));
    }

    quadrado3 = Rect(450, 350, 100, 100);
    rectangle(desc, quadrado3, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado3") != menuImages.end()){    
        Point iconPos(quadrado3.x + 10, quadrado3.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado3"], iconPos, Size(80, 80));
    }

    quadrado4 = Rect(450, 500, 100, 100);
    rectangle(desc, quadrado4, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado4") != menuImages.end()){    
        Point iconPos(quadrado4.x + 10, quadrado4.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado4"], iconPos, Size(80, 80));
    }

    quadrado5 = Rect(450, 650, 100, 100);
    rectangle(desc, quadrado5, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado5") != menuImages.end()){    
        Point iconPos(quadrado5.x + 10, quadrado5.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado5"], iconPos, Size(80, 80));
    }

    quadrado6 = Rect(600, 350, 100, 100);
    rectangle(desc, quadrado6, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado6") != menuImages.end()){    
        Point iconPos(quadrado6.x + 10, quadrado6.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado6"], iconPos, Size(80, 80));
    }

    quadrado7 = Rect(600, 500, 100, 100);
    rectangle(desc, quadrado7, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado7") != menuImages.end()){    
        Point iconPos(quadrado7.x + 10, quadrado7.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado7"], iconPos, Size(80, 80));
    }

    quadrado8 = Rect(600, 650, 100, 100);
    rectangle(desc, quadrado8, Scalar(255, 255, 255), 2);

    if(menuImages.find("quadrado8") != menuImages.end()){    
        Point iconPos(quadrado8.x + 10, quadrado8.y + 10);
        drawImageOnMenu(desc, menuImages["quadrado8"], iconPos, Size(80, 80));
    }

    botaoLeave = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
    rectangle(desc, botaoLeave, Scalar(255, 255, 255), 2);
    putText(desc, "Leave",
            Point(botaoLeave.x + 70, botaoLeave.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 3);
}

// Método para mostrar o menu principal
void Menu::showMainMenu() {
    Mat menu(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));
    drawMainMenu(menu);
    imshow(windowName, menu);
}

// Método para mostrar o menu de opções
void Menu::showOptionsMenu() {
    Mat options(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));
    drawOptionsMenu(options);
    imshow(windowName, options);
}

// Método para mostrar o menu de descrição
void Menu::showDescriptionMenu() {
    Mat desc(camHeight, camWidth, CV_8UC3, Scalar(0, 0, 0));
    drawDescriptionMenu(desc);
    imshow(windowName, desc);
}

// Callback estático do mouse
void Menu::mouseCallbackStatic(int event, int x, int y, int flags, void* userdata) {
    Menu* menu = static_cast<Menu*>(userdata);
    if (menu) {
        menu->mouseCallback(event, x, y, flags);
    }
}

// Callback do mouse da instância
void Menu::mouseCallback(int event, int x, int y, int flags) {
    if (estado == MENU && event == EVENT_LBUTTONDOWN) {
        if (botaoJogar.contains(Point(x, y))) {
            std::cout << "Jogar clicado!" << std::endl;
            estado = JOGO;
        }
        if (botaoSair.contains(Point(x, y))) {
            std::cout << "Sair clicado!" << std::endl;
            estado = SAIR;
        }
        if (botaoOpcoes.contains(Point(x, y))) {
            std::cout << "Options clicado!" << std::endl;
            estado = OPTIONS;
        }
        if (botaoDesc.contains(Point(x, y))) {
            std::cout << "Description clicado!" << std::endl;
            estado = DESC;
        }
    }
    if (estado == OPTIONS && event == EVENT_LBUTTONDOWN) {
        if (botaoLeave.contains(Point(x, y))) {
            std::cout << "Leave options clicado!" << std::endl;
            estado = MENU;
        }
    }
    if (estado == DESC && event == EVENT_LBUTTONDOWN) {
        if (botaoLeave.contains(Point(x, y))) {
            std::cout << "Leave desc clicado!" << std::endl;
            q = false;
            q1 = false;
            q2 = false;
            q3 = false;
            q4 = false;
            q5 = false;
            q6 = false;
            q7 = false;
            q8 = false;
            estado = MENU;
        }
        if(quadrado.contains(Point(x,y))){
            q = true;
            q1 = false;
            q2 = false;
            q3 = false;
            q4 = false;
            q5 = false;
            q6 = false;
            q7 = false;
            q8 = false;
        }
        if(quadrado1.contains(Point(x,y))){
            q = false;
            q1 = true;
            q2 = false;
            q3 = false;
            q4 = false;
            q5 = false;
            q6 = false;
            q7 = false;
            q8 = false;
        }
        if(quadrado2.contains(Point(x,y))){
            q = false;
            q1 = false;
            q2 = true;
            q3 = false;
            q4 = false;
            q5 = false;
            q6 = false;
            q7 = false;
            q8 = false;
        }
        if(quadrado3.contains(Point(x,y))){
            q = false;
            q1 = false;
            q2 = false;
            q3 = true;
            q4 = false;
            q5 = false;
            q6 = false;
            q7 = false;
            q8 = false;
        }
        if(quadrado4.contains(Point(x,y))){
            q = false;
            q1 = false;
            q2 = false;
            q3 = false;
            q4 = true;
            q5 = false;
            q6 = false;
            q7 = false;
            q8 = false;
        }
        if(quadrado5.contains(Point(x,y))){
            q = false;
            q1 = false;
            q2 = false;
            q3 = false;
            q4 = false;
            q5 = true;
            q6 = false;
            q7 = false;
            q8 = false;
        }
        if(quadrado6.contains(Point(x,y))){
            q = false;
            q1 = false;
            q2 = false;
            q3 = false;
            q4 = false;
            q5 = false;
            q6 = true;
            q7 = false;
            q8 = false;
        }
        if(quadrado7.contains(Point(x,y))){
            q = false;
            q1 = false;
            q2 = false;
            q3 = false;
            q4 = false;
            q5 = false;
            q6 = false;
            q7 = true;
            q8 = false;
        }
        if(quadrado8.contains(Point(x,y))){
            q = false;
            q1 = false;
            q2 = false;
            q3 = false;
            q4 = false;
            q5 = false;
            q6 = false;
            q7 = false;
            q8 = true;
        }
    }
    if (botaoJogar.contains(Point(x,y))){
        play = true;
    }else{
        play = false;
    }
    if(botaoOpcoes.contains(Point(x, y))){
        opcoes = true;
    }else{
        opcoes = false;
    }
    if(botaoDesc.contains(Point(x, y))){
        description = true;
    }else{
        description = false;
    }
    if(botaoSair.contains(Point(x, y))){
        sair = true;
    }else{
        sair = false;
    }
}

// Método para configurar o callback do mouse
void Menu::setupMouseCallback() {
    setMouseCallback(windowName, mouseCallbackStatic, this);
}