#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <stdbool.h>

#define MAX_SENHA_ARQUIVO 100
#define MAX_USUARIO_ARQUIVO 50
#define MAX_PRODUTO_ARQUIVO 100



void info_estoqueVoltar()
{
    info_estoque();
    voltar();
}
void alterar_quantidade( char *nome_produto, int nova_quantidade)
{
    FILE *f = fopen("estoque.txt", "r");
    FILE *temp = fopen("temp_Alterado_Estoque.txt", "w");
    char linha[500];
    int lim = 50;
    char setor_guardado[lim];
    char nome_guardado[lim];
    int qtd_guardada;
    int dia_guardado, mes_guardado, ano_guardado;

    if (f == NULL || temp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int encontrado = 0;

    while (fgets(linha, sizeof(linha), f) != NULL)
    {

        sscanf(linha, "%49[^:]:%49[^:]:%d:%d:%d:%d",setor_guardado, nome_guardado,&qtd_guardada, &dia_guardado, &mes_guardado, &ano_guardado) ;

        if (strcmp(nome_guardado,nome_produto) == 0)
        {
             // Atualiza a quantidade
            qtd_guardada = nova_quantidade;
            encontrado = 1;

        }

        fprintf(temp, "%s:%s:%d:%d:%d:%d\n", setor_guardado, nome_guardado, qtd_guardada,dia_guardado, mes_guardado, ano_guardado);
    }

    printf("Aperte:\n[S] SAIR\n[C] Continuar\n");
    fclose(f);
    fclose(temp);

    remove("estoque.txt");
    rename("temp_Alterado_Estoque.txt", "estoque.txt");

    char escolha = getch();


    if (escolha == 'c' || escolha == 'C')
    {
      limpar_tela();
       modificar_estoque();
    }
    else if (escolha == 's' || escolha == 'S')
    {
        menu(usuario);

    }
     else
     {
      printf("Opção inválida.\n");
     }


    if (!encontrado)
    {
        printf("Produto não encontrado.\n");

    }

}
void add_estoque(char *setor,char *nome,int quantidade,int dia,int mes,int ano)
{
   FILE *fpont;
   char linhas[MAX_PRODUTO_ARQUIVO + 3];
   fpont = fopen("estoque.txt", "a");
   if (fpont == NULL)
   {
    perror("Nao foi possivel abrir/criar o arquivo de produtos para adicionar");
    exit(1);
   }
    fprintf(fpont, "%s:%s:%d:%d:%d:%d\n", setor, nome, quantidade,dia,mes,ano);
    fclose(fpont);
    telas_carregamento();
    printf("Produto %s foi adiccionado com sucesso",nome);

    voltar();
    getch();
}
void del_estoque(char *nome)
{
    FILE *fpont, *temp;
    char linhas[MAX_PRODUTO_ARQUIVO + 3];
    bool encontrado = false;

    fpont = fopen("estoque.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fpont == NULL || temp == NULL)
    {
        perror("Erro ao abrir os arquivos.");
        exit(1);
    }

    while (fgets(linhas, sizeof(linhas), fpont) != NULL)
    {
        char setor_guardado[50], nome_guardado[50];
        int qtd_guardada, dia_guardado, mes_guardado, ano_guardado;

        if (sscanf(linhas, "%49[^:]:%49[^:]:%d:%d:%d:%d",setor_guardado, nome_guardado,&qtd_guardada, &dia_guardado, &mes_guardado, &ano_guardado) == 6)
        {

            if (strcmp(nome_guardado, nome) != 0)
            {
                fprintf(temp, "%s:%s:%d:%d:%d:%d\n",setor_guardado, nome_guardado,qtd_guardada, dia_guardado, mes_guardado, ano_guardado);
            }
            else
            {
                encontrado = true;
            }
        }
    }

    fclose(fpont);
    fclose(temp);

    remove("estoque.txt");
    rename("temp.txt", "estoque.txt");


    if (encontrado)
    {
         telas_carregamento();
        printf("\nProduto '%s' removido com sucesso.\n", nome);
    }
    else
    {
         telas_carregamento();
        printf("\nProduto '%s' nao encontrado no estoque.\n", nome);
    }
    voltar();
    getch();

}

void info_estoque()
{
   FILE *fpont;
   const int lim = 50;
   char linhas[MAX_PRODUTO_ARQUIVO + 3];

   fpont = fopen("estoque.txt", "r");
   if (fpont == NULL)
   {
       perror("Nao foi possivel abrir o arquivo de estoque");
       exit(1);
   }

   printf("\tEstoque\n\n");

   while (fgets(linhas, sizeof(linhas), fpont) != NULL)
   {
       char setor_guardado[lim];
       char nome_guardado[lim];
        int qtd_guardada;
       int dia_guardado, mes_guardado, ano_guardado;

       if (sscanf(linhas, "%49[^:]:%49[^:]:%d:%d:%d:%d",setor_guardado, nome_guardado,&qtd_guardada, &dia_guardado, &mes_guardado, &ano_guardado) == 6)
       {

           printf("Setor: %s\nNome: %s\nQuantidade: %d\nValidade: %02d/%02d/%d\n\n",setor_guardado, nome_guardado, qtd_guardada,dia_guardado, mes_guardado, ano_guardado);

       }



   }

   fclose(fpont);



}
void criar_lista_compras()
{
    FILE *fpont, *lista;
    char linhas[MAX_PRODUTO_ARQUIVO + 3];
    const int limite_minimo = 3;
    bool algum = false;

    fpont = fopen("estoque.txt", "r");
    lista = fopen("lista_compras.txt", "w");

    if (fpont == NULL || lista == NULL)
    {
        perror("Erro ao abrir os arquivos.");
        exit(1);
    }

    while (fgets(linhas, sizeof(linhas), fpont) != NULL)
    {
        char setor_guardado[50], nome_guardado[50];
        int qtd_guardada, dia_guardado, mes_guardado, ano_guardado;

        if (sscanf(linhas, "%49[^:]:%49[^:]:%d:%d:%d:%d",setor_guardado, nome_guardado,&qtd_guardada, &dia_guardado, &mes_guardado, &ano_guardado) == 6)
        {

            if (qtd_guardada <= limite_minimo)
            {
                fprintf(lista, "Setor: %s | Produto: %s | Quantidade: %d | Validade: %02d/%02d/%d\n",setor_guardado, nome_guardado, qtd_guardada,dia_guardado, mes_guardado, ano_guardado);
                algum = true;
            }
        }
    }

    fclose(fpont);
    fclose(lista);

    if (algum)
    {
        exibir_lista_compras();
        printf("\n\nLista de compras criada com sucesso em 'lista_compras.txt'\n");

        voltar();
    }
    else
    {
        printf("\nNenhum produto com estoque baixo.\n");
        voltar();
    }

    getch();
}
void exibir_lista_compras()
{
    FILE *f ;

    char linhas[MAX_PRODUTO_ARQUIVO + 3];
    f = fopen("lista_compras.txt", "r");

    printf("\n\n             LISTA DE COMPRAS\n");
   while (fgets(linhas, sizeof(linhas), f) != NULL)
   {

        printf("\n%s",linhas);

   }

   fclose(f);
   return;
}

void cadastro_usuario(char *usuario, char *senha)
{

    FILE *fpont;
    char linhas[MAX_USUARIO_ARQUIVO + MAX_SENHA_ARQUIVO + 3];


    fpont = fopen("usuarios.txt", "r");
    if (fpont != NULL)
    {
        while (fgets(linhas, sizeof(linhas), fpont) != NULL)
        {
            char nome_guardado[MAX_USUARIO_ARQUIVO];
            if (sscanf(linhas, "%[^:]", nome_guardado) == 1)
            {
                if (strcmp(usuario, nome_guardado) == 0)
                {
                    printf("\nUsuario %s ja existe.\n", usuario);
                    fclose(fpont);
                    Sleep(2000);
                    return;
                }
            }
        }
        fclose(fpont);
    }

    fpont = fopen("usuarios.txt", "a");
    if (fpont == NULL)
    {
        perror("Nao foi possivel abrir/criar o arquivo de usuarios para cadastro");
        exit(1);
    }

    fprintf(fpont, "%s:%s\n", usuario, senha);
    fclose(fpont);

    telas_carregamento();
    printf("\nUsuario %s cadastrado com sucesso!\n", usuario);
}


void login_usuario(char *usuario_digitado, char *senha_digitada)
{
    FILE *fpont;
    const int lim = MAX_USUARIO_ARQUIVO + MAX_SENHA_ARQUIVO + 3;
    char linhas[lim];
    bool logged_in = false;

    fpont = fopen("usuarios.txt", "r");
    if (fpont == NULL)
    {
        printf("Arquivo de usuarios nao encontrado ou sem permissoes.\n");
        return;
    }

    while (fgets(linhas, sizeof(linhas), fpont) != NULL)
    {
        char nome_guardado[MAX_USUARIO_ARQUIVO];
        char senha_guardada[MAX_SENHA_ARQUIVO];

        if (sscanf(linhas, "%49[^:]:%99[^\n]", nome_guardado, senha_guardada) == 2)
            {
             if (strcmp(usuario_digitado, nome_guardado) == 0)
                {
                 if (strcmp(senha_digitada, senha_guardada) == 0) {
                    printf("\nLogin bem-sucedido!\n");
                    logged_in = true;
                    fclose(fpont);
                    telas_carregamento();
                    menu(nome_guardado);
                    return;
                }
                else
                {
                    printf("\nSenha incorreta para o usuario %s.\n", usuario_digitado);
                    logged_in = true;
                    break;
                }
            }
        }
    }

    if (!logged_in)
    {
        printf("\nUsuario '%s' nao encontrado.\n", usuario_digitado);
    }
    fclose(fpont);
}
