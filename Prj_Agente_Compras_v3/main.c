#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
char usuario[50];
void telas_carregamento()
{
  int tempo = 0;
  int fim_carregar = 0;
  char* carregar;
    do
    {
    switch(tempo)
    {
    case 0:
        carregar = "Carregando   ";
        tempo = 1;
        break;
    case 1:
        carregar = "Carregando.  ";
        tempo = 2;
        break;
    case 2:
        carregar = "Carregando.. ";
        tempo = 3;
        break;
    case 3:
        carregar = "Carregando...";
        tempo = 0;
        break;
    }
    printf("\r%s",carregar);

    Sleep(500);
    fim_carregar++;
    }while(fim_carregar < 12);
}
void limpar_tela()
{
    system("cls");
}
void cadastro()
{
  const int lim_nome = 50;
    const int lim_senha = 100;
    char nome_usuario[lim_nome];
    char senha[lim_senha];
    char ver_senha[lim_senha];
    bool sucesso = false;

    do
    {
        limpar_tela();
        printf("\t\tCadastro\n\n");

        printf("Nome de usuario:");
        fgets(nome_usuario,sizeof(nome_usuario),stdin);
        nome_usuario[strcspn(nome_usuario, "\n")] = 0;

        printf("\nSenha: ");
        fgets(senha,sizeof(senha),stdin);
        senha[strcspn(senha, "\n")] = 0;

        printf("\nConfirme a senha: ");
        fgets(ver_senha,sizeof(ver_senha),stdin);
        ver_senha[strcspn(ver_senha, "\n")] = 0;

        if (strlen(senha) < 4)
            {
            printf("*A senha deve ter pelo menos 4 caracteres.\n");
            printf("Pressione qualquer tecla para tentar novamente.\n");
            getch();
            continue;
        }

        if(strcmp(senha,ver_senha)== 0)
        {
            sucesso = true;
        }else
        {
            printf("*As senhas nao sao identicas.\n");
            printf("Deseja tentar novamente?(S/N)");
            char ch = toupper(getch());
            if(ch == 'N')
            {
                printf("\n\n");
                telas_carregamento();
                limpar_tela();
                login();
                return;
            }
        }
    }while(!sucesso);

    cadastro_usuario(nome_usuario, senha);
    getch();
    limpar_tela();
    login();
}

void login()
{
    const int lim_nome = 50;
    const int lim_senha = 100;
    char nome_usuario[lim_nome];
    char senha[lim_senha];

    limpar_tela();
    printf("\t-----------\n");
    printf("\t|| Login ||\n");
    printf("\t-----------\n\n");

    printf("Nome de usuario: ");

    fgets(nome_usuario,sizeof(nome_usuario),stdin);
    nome_usuario[strcspn(nome_usuario, "\n")] = 0;
     strcpy(usuario, nome_usuario);
    printf("\nSenha: ");

    fgets(senha,sizeof(senha),stdin);
    senha[strcspn(senha, "\n")] = 0;

    login_usuario(nome_usuario, senha);
    getch();
    limpar_tela();
}
void tela_inicial()
{
    printf("----------------------\n");
    printf("|| 1.Login          ||\n");
    printf("|| 2.Cadastro       ||\n");
    printf("|| 3.Sair           ||\n");
    printf("----------------------\n");
    char ch = getch();
    switch(ch)
    {
    case '1':
        login();
        break;
    case '2':
        cadastro();
        break;
    default:
        exit(0);
        break;
    }
}
void menu(char *usuario)
{
    limpar_tela();
    printf("---------------------------------------\n");
    printf("       Bem Vindo %s                 \n",usuario);
    printf("---------------------------------------\n");
    printf("||   1.Verificar estoque             ||\n");
    printf("||   2.Alterar Unidades do Estoque   ||\n");
    printf("||   3.Adicionar produto             ||\n");
    printf("||   4.Remover produto               ||\n");
    printf("||   5.Criar uma lista de compras    ||\n");
    printf("---------------------------------------\n");

    char ch = getch();
    switch(ch)
    {
    case '1':
        limpar_tela();
        ver_estoque();
        break;
    case '2':
        limpar_tela();
        modificar_estoque();
        break;
    case '3':
        limpar_tela();
        add_produto();
        break;
    case '4':
        limpar_tela();
        del_produto();
        break;
    case '5':
        telas_carregamento();
        limpar_tela();
        criacao_lista();
        break;
    default:
        printf("Opção invalida!");
        break;
    }
}

int main()
{
    tela_inicial();
    return 0;
}
void pausar(int tempo_ms) {
    #ifdef _WIN32
        Sleep(tempo_ms);
    #else
        usleep(tempo_ms * 1000);
    #endif
}
