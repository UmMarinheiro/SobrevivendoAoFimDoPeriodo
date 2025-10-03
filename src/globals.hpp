#pragma once

enum Estado { MENU, JOGO, RODAR_JOGO, SAIR_JOGO, SAIR, OPTIONS, DESC, LARANJA };

extern Estado estado;

enum Turno { PHOTO, ITEM, POSITION };

extern Turno turno;
