#pragma once

enum Estado { MENU, JOGO, SAIR, OPTIONS, DESC };

extern Estado estado;

enum Turno { PHOTO, ITEM, POSITION, PARRY };

extern Turno turno;