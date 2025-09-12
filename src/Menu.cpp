#include "Menu.hpp"
#include <iostream>

Menu::Menu(int width, int height, const std::string& wName) 
    : camWidth(width), camHeight(height), windowName(wName) {
}
Menu::~Menu(){}

// Método para desenhar o menu principal
void Menu::drawMainMenu(Mat& menu) {
    // Título
    putText(menu, "Sobrevivendo Ao Fim Do Periodo",
            Point(camWidth * 0.25 - 35, camHeight * 0.25),
            FONT_HERSHEY_TRIPLEX, 2.0,
            Scalar(255, 255, 255), 3);

    // Botão Jogar
    botaoJogar = Rect(camWidth * 0.5 - 90, camHeight * 0.5 - 50, 240, 60);
    rectangle(menu, botaoJogar, Scalar(255, 255, 255), 2);
    putText(menu, "Play",
            Point(botaoJogar.x + 85, botaoJogar.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 2);

    // Botão Opções
    botaoOpcoes = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 50, 240, 60);
    rectangle(menu, botaoOpcoes, Scalar(255, 255, 255), 2);
    putText(menu, "Options",
            Point(botaoOpcoes.x + 55, botaoOpcoes.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 2);

    // Botão Description
    botaoDesc = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 150, 240, 60);
    rectangle(menu, botaoDesc, Scalar(255, 255, 255), 2);
    putText(menu, "Description",
            Point(botaoDesc.x + 20, botaoDesc.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 2);

    // Botão Sair
    botaoSair = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
    rectangle(menu, botaoSair, Scalar(255, 255, 255), 2);
    putText(menu, "Exit",
            Point(botaoSair.x + 75, botaoSair.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 2);
}

// Método para desenhar o menu de opções
void Menu::drawOptionsMenu(Mat& options) {
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
    putText(desc, "Description",
            Point(camWidth * 0.25 + 200, camHeight * 0.25),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 3);

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
            estado = MENU;
        }
    }
}

// Método para configurar o callback do mouse
void Menu::setupMouseCallback() {
    setMouseCallback(windowName, mouseCallbackStatic, this);
}