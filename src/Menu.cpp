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
    Mat bgMain = imread("assets/arrascaeta.jpeg", IMREAD_COLOR);
    if (!bgMain.empty()) {
        menuImages["main_bg"] = bgMain;
    }
    
    Mat bgOptions = imread("assets/naogrita.jpeg", IMREAD_COLOR);
    if (!bgOptions.empty()) {
        menuImages["options_bg"] = bgOptions;
    }
    
    // Carrega ícones/logos
    Mat logo = imread("assets/game_logo.png", IMREAD_UNCHANGED);
    if (!logo.empty()) {
        menuImages["logo"] = logo;
    }
    Mat descriptionIcon = imread("assets/desc_icon.png", IMREAD_UNCHANGED);
    if (!descriptionIcon.empty()) {
        menuImages["description_icon"] = descriptionIcon;
    }

    // Carrega ícones dos botões
    Mat playIcon = imread("assets/orange.png", IMREAD_UNCHANGED);
    if (!playIcon.empty()) {
        menuImages["play_icon"] = playIcon;
    }
    
    Mat optionsIcon = imread("assets/options_icon.png", IMREAD_UNCHANGED);
    if (!optionsIcon.empty()) {
        menuImages["options_icon"] = optionsIcon;
    }
    
    Mat exitIcon = imread("assets/exit_icon.png", IMREAD_UNCHANGED);
    if (!exitIcon.empty()) {
        menuImages["exit_icon"] = exitIcon;
    }
    
    Mat descIcon = imread("assets/desc_icon.png", IMREAD_UNCHANGED);
    if (!descIcon.empty()) {
        menuImages["desc_icon"] = descIcon;
    }
    Mat descOrange = imread("assets/orange.png", IMREAD_COLOR);
    if (!descOrange.empty()) {
        menuImages["desc_orange"] = descOrange;
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
    if (menuImages.find("play_icon") != menuImages.end()) {
        Point iconPos(botaoJogar.x + 10, botaoJogar.y + 10);
        drawImageOnMenu(menu, menuImages["play_icon"], iconPos, Size(40, 40));
        // Texto ao lado do ícone
        putText(menu, "Play",
                Point(botaoJogar.x + 60, botaoJogar.y + 40),
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
    
    if (menuImages.find("options_icon") != menuImages.end()) {
        Point iconPos(botaoOpcoes.x + 10, botaoOpcoes.y + 10);
        drawImageOnMenu(menu, menuImages["options_icon"], iconPos, Size(40, 40));
        putText(menu, "Options",
                Point(botaoOpcoes.x + 60, botaoOpcoes.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    } else {
        putText(menu, "Options",
                Point(botaoOpcoes.x + 55, botaoOpcoes.y + 40),
                FONT_HERSHEY_TRIPLEX, 1.0,
                Scalar(255, 255, 255), 2);
    }

    // Botão Description
    botaoDesc = Rect(camWidth * 0.5 - 120, camHeight * 0.5 + 150, 240, 60);
    rectangle(menu, botaoDesc, Scalar(255, 255, 255), 2);
    
    if (menuImages.find("desce_icon") != menuImages.end()) {
        Point iconPos(botaoDesc.x + 10, botaoDesc.y + 10);
        drawImageOnMenu(menu, menuImages["desc_icon"], iconPos, Size(40, 40));
        putText(menu, "Description",
                Point(botaoDesc.x + 60, botaoDesc.y + 40),
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
    
    if (menuImages.find("exit_icon") != menuImages.end()) {
        Point iconPos(botaoSair.x + 10, botaoSair.y + 10);
        drawImageOnMenu(menu, menuImages["exit_icon"], iconPos, Size(40, 40));
        putText(menu, "Exit",
                Point(botaoSair.x + 60, botaoSair.y + 40),
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
    if (laranja == true){
        putText(desc, "AQUI EH FRAMENGO PORRA!!!",
            Point(camWidth * 0.68 - 1, camHeight * 0.25),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 3);
    }

    linhaVertical = Rect(camWidth * 0.65 - 1, camHeight * 0.2, 2, camHeight * 0.5);
    rectangle(desc, linhaVertical, Scalar(255, 255, 255), FILLED);

    quadrado = Rect(300, 350, 100, 100);
    rectangle(desc, quadrado, Scalar(255, 255, 255), 2);

    if (menuImages.find("desc_orange") != menuImages.end()) {
        Point iconPos(quadrado.x + 10, quadrado.y + 10);
        drawImageOnMenu(desc, menuImages["desc_orange"], iconPos, Size(80, 80));
    }
    
    // Você pode adicionar uma imagem de descrição/instrução aqui
    if (menuImages.find("instructions") != menuImages.end()) {
        Point imgPos(camWidth * 0.1, camHeight * 0.4);
        drawImageOnMenu(desc, menuImages["instructions"], imgPos, Size(camWidth * 0.8, camHeight * 0.3));
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
            laranja = false;
            estado = MENU;
        }
        if(quadrado.contains(Point(x,y))){
            std::cout << "Quadrado clicado!" << std::endl;
            laranja = true;
        }
    }
    /*if (quadrado.contains(Point(x,y))){
        std::cout << "Jogar hover!" << std::endl; LOGICA DOS BOTOES DO MENU!!!
        laranja = true;
    }*/ 
}

// Método para configurar o callback do mouse
void Menu::setupMouseCallback() {
    setMouseCallback(windowName, mouseCallbackStatic, this);
}