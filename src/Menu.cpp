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
    inicializarItensDescricao();
}

int v = 5;

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
//=====================================================================================================================
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
//=====================================================================================================================
void Menu::loadMenuImages() {
    Mat bgMain = imread("assets/images/bck.png", IMREAD_COLOR);
    if (!bgMain.empty()) {
        menuImages["main_bg"] = bgMain;
    }
    
    Mat bgOptions = imread("assets/images/options_bg.png", IMREAD_COLOR);
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
    
    Mat quadradoIcon = imread("assets/images/arrascaeta.jpeg", IMREAD_UNCHANGED);
    if(!quadradoIcon.empty()){
        menuImages["quadrado"] = quadradoIcon;
    }
    Mat volumePlus = imread("assets/images/plus.png", IMREAD_UNCHANGED);
    if(!volumePlus.empty()){
        menuImages["volumePlus"] = volumePlus;
    }
    Mat volumeMinus = imread("assets/images/minus.png", IMREAD_UNCHANGED);
    if(!volumeMinus.empty()){
        menuImages["volumeMinus"] = volumeMinus;
    }
//===================================================ICONES DOS ITENS=================================================================
    Mat Mochila = imread("assets/itens/mochila.png", IMREAD_UNCHANGED);
    if(!Mochila.empty()){
        menuImages["Mochila"] = Mochila;
    }
    Mat Marmita = imread("assets/itens/marmita.png", IMREAD_UNCHANGED);
    if(!Marmita.empty()){  
        menuImages["Marmita"] = Marmita;
    }
    Mat Liga = imread("assets/itens/liga.png", IMREAD_UNCHANGED);
    if(!Liga.empty()){  
        menuImages["Liga"] = Liga;
    }
}
//=====================================================================================================================
void Menu::initializeButtonAnimations() {
    animBotaoJogar.setStaticFrame(Scalar(90, 50, 30));
    animBotaoJogar.hoverAnimation.generatePulseFrames(240, 60, Scalar(90, 50, 30), 10);
    animBotaoJogar.hoverAnimation.setFrameDuration(0.06);
    
    animBotaoOpcoes.setStaticFrame(Scalar(90, 50, 30));
    animBotaoOpcoes.hoverAnimation.generatePulseFrames(240, 60, Scalar(90, 50, 30), 10);
    animBotaoOpcoes.hoverAnimation.setFrameDuration(0.06);
    
    animBotaoDesc.setStaticFrame(Scalar(90, 50, 30));
    animBotaoDesc.hoverAnimation.generatePulseFrames(240, 60, Scalar(90, 50, 30), 10);
    animBotaoDesc.hoverAnimation.setFrameDuration(0.06);
    
    animBotaoSair.setStaticFrame(Scalar(90, 50, 30));
    animBotaoSair.hoverAnimation.generatePulseFrames(240, 60, Scalar(90, 50, 30), 10);
    animBotaoSair.hoverAnimation.setFrameDuration(0.06);
    
    animBotaoLeave.setStaticFrame(Scalar(90, 50, 30));
    animBotaoLeave.hoverAnimation.generatePulseFrames(240, 60, Scalar(90, 50, 30), 10);
    animBotaoLeave.hoverAnimation.setFrameDuration(0.06);
}
//=====================================================================================================================
void Menu::updateAnimations() {
    animBotaoJogar.updateAnimation();
    animBotaoOpcoes.updateAnimation();
    animBotaoDesc.updateAnimation();
    animBotaoSair.updateAnimation();
    animBotaoLeave.updateAnimation();
}
void Menu::inicializarItensDescricao() {
    itensDescricao.clear();
    
    // Definir todas as posições dos quadrados primeiro
    vector<Rect> posicoes = {
        Rect(450, 350, 100, 100),
        Rect(600, 350, 100, 100),
        Rect(750, 350, 100, 100),
        Rect(900, 350, 100, 100),
        Rect(1050, 350, 100, 100),
        Rect(450, 500, 100, 100)
    };
    
    // Adicionar todos os itens possíveis (inicialmente bloqueados)
    itensDescricao = {
        {"Mochila", "Mochila", "Item essencial para carregar materiais academicos", false, posicoes[0]},
        {"Marmita", "Marmita", "Refeicao caseira para manter o foco nas aulas", false, posicoes[1]},
        {"Liga", "Liga", "Otima para guardar dinheiro, mas tambem para atirar nos colegas", false, posicoes[2]},
        //{"Calculadora", "Calculadora", "Essencial para calculos complexos", false, posicoes[4]},
        //{"Garrafa", "Garrafa de Agua", "Hidratacao é fundamental para o aprendizado", false, posicoes[5]}
    };
}

void Menu::desbloquearItem(const string& itemKey) {
    for (auto& item : itensDescricao) {
        if (item.key == itemKey && !item.desbloqueado) {
            item.desbloqueado = true;
            cout << "Item desbloqueado: " << item.nome << endl;
            break;
        }
    }
}

bool Menu::isItemDesbloqueado(const string& itemKey) const {
    for (const auto& item : itensDescricao) {
        if (item.key == itemKey) {
            return item.desbloqueado;
        }
    }
    return false;
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
//=====================================================================================================================
void Menu::drawMenu(Mat &desc, const std::vector<MenuItem>& items) {
    for (const auto& item : items) {
        // Desenha o quadrado
        rectangle(desc, item.box, Scalar(250, 255, 120), 2);

        // Se existir imagem associada
        auto it = menuImages.find(item.imageKey);
        if (it != menuImages.end()) {
            Point iconPos(item.box.x + 10, item.box.y + 10);
            Size iconSize(item.box.width - 20, item.box.height - 20);

            drawImageOnMenu(desc, it->second, iconPos, iconSize);
        }
    }
}

//=====================================================================================================================
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
    } /*else {
        putText(menu, "Sobrevivendo Ao Fim Do Periodo",
                Point(camWidth * 0.25 - 35, camHeight * 0.25),
                FONT_HERSHEY_TRIPLEX, 2.0,
                Scalar(255, 255, 255), 3);
    }*/

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
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(250, 255, 120), 2);

    putText(menu, "Options", Point(botaoOpcoes.x + 60, botaoOpcoes.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(250, 255, 120), 2);

    putText(menu, "Description", Point(botaoDesc.x + 20, botaoDesc.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(250, 255, 120), 2);

    putText(menu, "Exit", Point(botaoSair.x + 85, botaoSair.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(250, 255, 120), 2);
}
//=====================================================================================================================
void Menu::drawOptionsMenu(Mat& options) {
    if (menuImages.find("options_bg") != menuImages.end()) {
        Mat background = menuImages["options_bg"];
        resize(background, background, Size(camWidth, camHeight));
        background.copyTo(options);
    }
    
        volumep = Rect(450, 550, 50, 50);
        volumem = Rect(200, 550, 50, 50);
        
    if (menuImages.find("volumePlus") != menuImages.end()) {
        Mat volumePlus = menuImages["volumePlus"];
        Point volumePlusPos(450, 500);
        drawImageOnMenu(options, volumePlus, volumePlusPos, Size(300, 100));
    } else {
        putText(options, "+",
                Point(450, 600),
                FONT_HERSHEY_TRIPLEX, 2.0,
                Scalar(250, 255, 120), 3);
    }
    if (menuImages.find("volumeMinus") != menuImages.end()) {
        Mat volumeMinus = menuImages["volumeMinus"];
        Point volumeMinusPos(200, 600);
        drawImageOnMenu(options, volumeMinus, volumeMinusPos, Size(300, 100));
    } else {
        putText(options, "-",
                Point(200, 600),
                FONT_HERSHEY_TRIPLEX, 2.0,
                Scalar(250, 255, 120), 3);
    }
    vector<Rect> quadrados = {
        Rect(255, 560, 10, 40),
        Rect(275, 560, 10, 40),
        Rect(295, 560, 10, 40),
        Rect(315, 560, 10, 40),
        Rect(335, 560, 10, 40),
        Rect(355, 560, 10, 40),
        Rect(375, 560, 10, 40),
        Rect(395, 560, 10, 40),
        Rect(415, 560, 10, 40),
        Rect(435, 560, 10, 40)
    };

    for (int i = 0; i < 10; i++) {
        rectangle(options, quadrados[i], Scalar(100, 100, 100), FILLED);
    }

    for (int i = 0; i < v; i++) {
        rectangle(options, quadrados[i], Scalar(250, 255, 120), FILLED);
    }

    rectangle(options, Rect(750, 550, 75, 75), Scalar(250, 255, 120), 2);
    rectangle(options, Rect(750, 650, 75, 75), Scalar(250, 255, 120), 2);
    rectangle(options, Rect(1200, 550, 75, 75), Scalar(250, 255, 120), 2);
    rectangle(options, Rect(1200, 650, 75, 75), Scalar(250, 255, 120), 2);

    putText(options, "M", Point(760, 610),
            FONT_HERSHEY_TRIPLEX, 2.0,
            Scalar(255, 255, 255), 2);

    putText(options, "Voltar ao menu", Point(845, 600),
            FONT_HERSHEY_TRIPLEX, 0.8,
            Scalar(250, 255, 120), 2);

    putText(options, "Esc", Point(760, 700),
            FONT_HERSHEY_TRIPLEX, 1.0,
            Scalar(255, 255, 255), 2);
    
    putText(options, "Sair do jogo", Point(845, 690),
            FONT_HERSHEY_TRIPLEX, 0.8,
            Scalar(250, 255, 120), 2);

    putText(options, "J", Point(1220, 610),
            FONT_HERSHEY_TRIPLEX, 2.0,
            Scalar(255, 255, 255), 2);
    
    putText(options, "Aumentar volume", Point(1295, 600),
            FONT_HERSHEY_TRIPLEX, 0.8,
            Scalar(250, 255, 120), 2);

    putText(options, "K", Point(1215, 705),
            FONT_HERSHEY_TRIPLEX, 2.0,
            Scalar(255, 255, 255), 2);

    putText(options, "Diminuir volume", Point(1295, 690),
            FONT_HERSHEY_TRIPLEX, 0.8,
            Scalar(250, 255, 120), 2);

    animBotaoLeave.rect = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
    botaoLeave = animBotaoLeave.rect;

    Mat leaveSprite = animBotaoLeave.getCurrentSprite();
    leaveSprite.copyTo(options(botaoLeave));

    putText(options, "Volume", Point(290, 530),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(250, 255, 120), 2);
    
    putText(options, "Leave", Point(botaoLeave.x + 70, botaoLeave.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(250, 255, 120), 3);
    
}
//=====================================================================================================================
void Menu::drawDescriptionMenu(Mat& desc) {
    
    if (menuImages.find("mopa.png") != menuImages.end()) {
        Mat background = menuImages["options_bg"];
        resize(background, background, Size(camWidth, camHeight));
        background.copyTo(desc);
    }

    // Linha divisória
    linhaVertical = Rect(camWidth * 0.65 - 1, camHeight * 0.2, 2, camHeight * 0.5);
    rectangle(desc, linhaVertical, Scalar(250, 255, 120), FILLED);

    // Desenhar apenas os itens desbloqueados
    vector<MenuItem> itemsParaDesenhar;
    for (const auto& item : itensDescricao) {
        if (item.desbloqueado) {
            itemsParaDesenhar.push_back({item.posicao, item.key});
        }
    }

    drawMenu(desc, itemsParaDesenhar);

    Point descPos(camWidth * 0.68, camHeight * 0.25);
    
    if (qselected > 0) {
    // Encontrar o item selecionado baseado na posição na lista de desbloqueados
    int contador = 1;
    bool itemEncontrado = false;
    
    for (const auto& item : itensDescricao) {
        if (item.desbloqueado) {
            if (contador == qselected) {
                putText(desc, item.nome, 
                        Point(descPos.x, descPos.y), 
                        FONT_HERSHEY_TRIPLEX, 0.8, Scalar(255, 255, 255), 2);
                
                string descricao = item.descricao;
                putText(desc, descricao, 
                        Point(descPos.x, descPos.y + 40), 
                        FONT_HERSHEY_SIMPLEX, 0.6, Scalar(200, 200, 200), 1);
                itemEncontrado = true;
                break;
            }
            contador++;
        }
    }
    
    // Se não encontrou o item selecionado, resetar qselected
    if (!itemEncontrado) {
        qselected = 0;
    }
    }   else {
        if (itemsParaDesenhar.empty()) {
            putText(desc, "Nenhum item desbloqueado ainda", 
                    descPos, FONT_HERSHEY_TRIPLEX, 0.7, Scalar(150, 150, 150), 2);
            putText(desc, "Explore o jogo para encontrar itens!", 
                    Point(descPos.x, descPos.y + 40), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(150, 150, 150), 1);
        } else {
            putText(desc, "Clique em um item para ver sua descricao", 
                    descPos, FONT_HERSHEY_TRIPLEX, 0.7, Scalar(255, 255, 255), 2);
        }
    }

    // Botão leave
    animBotaoLeave.rect = Rect(camWidth * 0.5 - 90, camHeight * 0.5 + 250, 240, 60);
    botaoLeave = animBotaoLeave.rect;

    Mat leaveSprite = animBotaoLeave.getCurrentSprite();
    leaveSprite.copyTo(desc(botaoLeave));

    putText(desc, "Leave", Point(botaoLeave.x + 70, botaoLeave.y + 40),
            FONT_HERSHEY_TRIPLEX, 1.0, Scalar(250, 255, 120), 3);
}
//=====================================================================================================================
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
            v++;
            if(v > 10) v = 10;
        }
        if(volumem.contains(Point(x, y))){
            playClickSound();
            audio.setSoundVolume(max(audio.getSoundVolume() - 10, 0));
            v--;
            if(v < 0) v = 0;
        }
    }
    
    if (estado == DESC && event == EVENT_LBUTTONDOWN) {
        if (botaoLeave.contains(Point(x, y))) {
            playClickSound();
            qselected = 0;
            estado = MENU;
        }
        int contador = 1;
        for (const auto& item : itensDescricao) {
            if (item.desbloqueado && item.posicao.contains(Point(x, y))) {
                playClickSound();
                qselected = contador;
                break;
            }
            if (item.desbloqueado) {
                contador++;
            }
        }
    }
}
void Menu::setupMouseCallback() {
    setMouseCallback(windowName, mouseCallbackStatic, this);
}