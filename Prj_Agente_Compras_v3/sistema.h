#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Funções utilitárias
void pausar(int tempo_ms);
void limpar_tela();
void telas_carregamento();
void voltar();

// Sistema de login e cadastro
void tela_inicial();
void cadastro();
void login();


void menu(usuario);


// Gerenciamento de estoque

void ver_estoque();
void add_produto();
void del_produto();
void info_estoque();
void info_estoqueVoltar();
// Lista de compras
void criacao_lista();

// Funções externas (implementadas em outros arquivos)
extern char usuario[50];

void criar_lista_compras();

#endif

