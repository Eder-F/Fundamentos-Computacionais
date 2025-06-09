#ifndef JOGO_H
#define JOGO_H

void loginUser();
void menu();
void cena(int cenaAtual);
void mapa(int mapaAtual);
void mostrarDialogo(int cena, int mapa);
void salvarJogo();
int verificarInimigo(char inimigo);
int atacarInimigo(char inimigo);

#endif
