#pragma once

#include "opencv2/opencv.hpp"
#include "globals.hpp"
#include <string>

using namespace cv;

class Menu {
private:

    int camWidth;
    int camHeight;
    
    // Botões do menu
    Rect botaoJogar;
    Rect botaoSair;
    Rect botaoOpcoes;
    Rect botaoDesc;
    Rect botaoLeave;
    
    std::string windowName;
    
    // Métodos para desenhar cada tela
    void drawMainMenu(Mat& menu);
    void drawOptionsMenu(Mat& options);
    void drawDescriptionMenu(Mat& desc);
    
public:
    Menu(int width, int height, const std::string& wName);

    ~Menu();
    
    // Métodos públicos
    void showMainMenu();
    void showOptionsMenu();
    void showDescriptionMenu();
    
    // Callback do mouse
    static void mouseCallbackStatic(int event, int x, int y, int flags, void* userdata);
    void mouseCallback(int event, int x, int y, int flags);
    
    // Getters para os botões (para verificação de clique)
    const Rect& getBotaoJogar() const { return botaoJogar; }
    const Rect& getBotaoSair() const { return botaoSair; }
    const Rect& getBotaoOpcoes() const { return botaoOpcoes; }
    const Rect& getBotaoDesc() const { return botaoDesc; }
    const Rect& getBotaoLeave() const { return botaoLeave; }
    
    // Método para configurar callback do mouse
    void setupMouseCallback();
};