#pragma once

#include "opencv2/opencv.hpp"
#include "globals.hpp"
#include <string>
#include <map>
#include <vector>

using namespace cv;

class Menu {
private:
    int camWidth;
    int camHeight;

    bool laranja = false;
    bool oscar = false;
    bool play = false;
    bool opcoes = false;
    bool description = false;
    bool sair = false;
    bool clear = false;

    //Variaveis auxiliares dos quadrados da descrição

    bool q = false;
    bool q1 = false;
    bool q2 = false;
    bool q3 = false;
    bool q4 = false;
    bool q5 = false;
    bool q6 = false;
    bool q7 = false;
    bool q8 = false;

    
    // Botões do menu
    Rect botaoJogar;
    Rect botaoSair;
    Rect botaoOpcoes;
    Rect botaoDesc;
    Rect botaoLeave;
    Rect linhaVertical;
    
    Rect quadrado;
    Rect quadrado1;
    Rect quadrado2;
    Rect quadrado3;
    Rect quadrado4;
    Rect quadrado5;
    Rect quadrado6;
    Rect quadrado7;
    Rect quadrado8;


    std::string windowName;
    
    // Armazenamento de imagens para os menus
    std::map<std::string, Mat> menuImages;
    
    // Métodos para desenhar cada tela
    void drawMainMenu(Mat& menu);
    void drawOptionsMenu(Mat& options);
    void drawDescriptionMenu(Mat& desc);
    
    // Método para desenhar imagem com transparência
    void drawImageOnMenu(Mat& menu, const Mat& image, Point position, Size targetSize = Size(0, 0));
    
    // Método para carregar todas as imagens dos menus
    void loadMenuImages();
    
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