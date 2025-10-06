#pragma once

#include "opencv2/opencv.hpp"
#include "globals.hpp"
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include "animatedSprite.hpp"
#include "audioManager.hpp"
#include <fstream>

using namespace cv;

struct MenuItem {
    Rect box;
    std::string imageKey;         
};

struct ItemDescricao {
    std::string key;
    std::string nome;
    std::string descricao;
    bool desbloqueado;
    Rect posicao;
};

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

    int qselected = 0;
    
    // Botões animados
    AnimatedButton animBotaoJogar;
    AnimatedButton animBotaoOpcoes;
    AnimatedButton animBotaoDesc;
    AnimatedButton animBotaoSair;
    AnimatedButton animBotaoLeave;
    
    // Botões do menu (mantidos para compatibilidade)
    Rect botaoJogar;
    Rect botaoSair;
    Rect botaoOpcoes;
    Rect botaoDesc;
    Rect botaoLeave;
    Rect linhaVertical;

    Rect volumep;
    Rect volumem;

    std::string windowName;

    bool wasJogarHovered;
    bool wasOpcoesHovered;
    bool wasDescHovered;
    bool wasSairHovered;
    bool wasLeaveHovered;
    
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
    void drawMenu(Mat &desc, const std::vector<MenuItem>& items);

    std::vector<ItemDescricao> itensDescricao;

    void inicializarItensDescricao();
    
    // Inicializar animações dos botões
    void initializeButtonAnimations();
    
public:
    Menu(int width, int height, const std::string& wName);
    ~Menu();
    
    // Métodos públicos
    void showMainMenu();
    void showOptionsMenu();
    void showDescriptionMenu();
    
    // Atualizar animações
    void updateAnimations();
    
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

    //audio

    void playHoverSound();
    void playClickSound();
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void playSound(const std::string& filePath);

    //Itens

    void desbloquearItem(const std::string& itemKey);
    bool isItemDesbloqueado(const std::string& itemKey) const;
};