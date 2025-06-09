#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h> // para sleep()
#include "jogo.h"

#define MAX 100
char inimigo;
int encontrou;
char usuarioLogado[MAX];
int posX = 1, posY = 1;
int cenaAtual = 1, mapaAtual = 1;

void limparTela()
{
    system("cls");
}

void loginUser()
{
    FILE *f = fopen("usuario.csv", "a+");
    char nome[MAX], senha[MAX], linha[MAX], nomeLido[MAX], senhaLida[MAX];
    int encontrado = 0;

    printf("Nome de usuario: ");
    scanf("%s", nome);
    printf("Senha: ");
    scanf("%s", senha);

    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f))
    {
        sscanf(linha, "%[^,],%s", nomeLido, senhaLida);
        if (strcmp(nome, nomeLido) == 0 && strcmp(senha, senhaLida) == 0)
        {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        fprintf(f, "%s,%s\n", nome, senha);
        printf("Usuario criado!\n");
    }
    else
    {
        printf("Login bem-sucedido!\n");
    }

    strcpy(usuarioLogado, nome);
    fclose(f);
}

void menu()
{
    FILE *f = fopen("saves.csv", "r");
    int temSave = 0;
    char linha[MAX], nomeLido[MAX];

    while (fgets(linha, MAX, f))
    {
        sscanf(linha, "%[^,],", nomeLido);
        if (strcmp(usuarioLogado, nomeLido) == 0)
        {
            sscanf(linha, "%*[^,],%d,%d,%d,%d", &cenaAtual, &mapaAtual, &posX, &posY);
            temSave = 1;
            break;
        }
    }
    if (f) fclose(f);


    printf("\n=== Menu Principal ===\n");
    if (temSave) printf("1. Continuar\n");
    printf("2. Novo Jogo\n3. Sair\nEscolha: ");
    char escolha = getch();

    if (temSave && escolha == '1')
    {
        cena(cenaAtual);
    }
    else if (escolha == '2')
    {
        cenaAtual = 0;
        mapaAtual = 0;
        posX = 35;
        posY = 10;
        //salvarJogo();
        cena(cenaAtual);
    }
    else if (escolha == '3')
    {
        printf("Saindo...\n");
    }
}

void cena(int cena)
{
    mostrarDialogo(cena, mapaAtual);
   if(cena == 0)
   {
    mapaAtual = 1;
    cenaAtual = 1;
   }
    mapa(mapaAtual);

}

void mostrarDialogo(int cena, int mapa)
{
    limparTela();
    FILE *f = fopen("dialogos.csv", "r");
    if (!f) return;

    char linha[200], texto[500];
    int cenaArquivo, mapaArquivo;

    fgets(linha, 200, f);
    while (fgets(linha, 200, f))
    {
        sscanf(linha, "%d,%d,\"%[^\"]\"", &cenaArquivo, &mapaArquivo, texto);

        if (cenaArquivo == cena && mapaArquivo == mapa)
        {
            printf("\n%s\n", texto);
        }
    }
    fclose(f);
    Sleep(2500);
}

void mapa(int mapaID)
{
    FILE *f = fopen("mapas.csv", "r");
    if (!f)
    {
        printf("Erro ao abrir mapas.csv\n");
        return;
    }

    char linha[1024];
    int mapaAtualID;
    char nome[50];
    int achou = 0;

    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f))
    {
        // Verifica se é o mapa certo
        sscanf(linha, "%d,%49[^,],", &mapaAtualID, nome);
        if (mapaAtualID != mapaID) continue;


        char *conteudoInicio = strchr(linha, '"');
        if (!conteudoInicio) continue;

        // Lê todo o conteúdo entre aspas
        char conteudo[5000] = "";
        strcat(conteudo, conteudoInicio + 1); // começa depois da primeira aspas

        // Continua lendo até fechar aspas
        while (!strchr(conteudo, '"') && fgets(linha, sizeof(linha), f))
        {
            strcat(conteudo, linha);
        }

        // Remove a última aspas (se houver)
        char *fim = strrchr(conteudo, '"');
        if (fim) *fim = '\0';

        achou = 1;

        // Imprimir o mapa com o jogador
        limparTela();
        printf("Mapa: %s\n", nome);
        int i = 0;
        int x = 0, y = 0;
        for (i; conteudo[i] != '\0'; i++)
        {
            if (x == posX && y == posY)
            {
                putchar('@');
            }

            else
            {
                putchar(conteudo[i]);
            }

            if (conteudo[i] == '\n')
            {
                y++;
                x = 0;
            }
            else
            {
                x++;
            }
        }

        break;
    }

    fclose(f);

    if (!achou)
    {
        printf("Mapa %d não encontrado.\n", mapaID);
        return;
    }

    // Entrada do jogador
    char cmd;
    printf("\n[WASD] mover, [P] atacar, [Q] sair\n> ");
    //scanf(" %c", &cmd);

    cmd =getch();
    if (cmd == 'q') return;


    int novoX = posX;
    int novoY = posY;
    if (cmd == 'w') novoY--;
    if (cmd == 's') novoY++;
    if (cmd == 'a') novoX--;
    if (cmd == 'd') novoX++;

    printf("%c\n",inimigo);
    if(cmd == 'p'&& encontrou == 1)
    {

        if(verificarInimigo(inimigo))
        {
            atacarInimigo(inimigo);
        }
        else
        {
            encontrou=0;
            printf("Nenhum inimigo!\n");
            inimigo=' ';
        }



    }

    if(cmd == 'p'&& encontrou == 0)
    {

        printf("Nenhum inimigo!\n");
        Sleep(1000);
    }

    // Verificar se pode andar
    FILE *f2 = fopen("mapas.csv", "r");
    fgets(linha, sizeof(linha), f2); // pular cabeçalho
    char conteudoVer[5000] = "";
    while (fgets(linha, sizeof(linha), f2))
    {
        int id;
        sscanf(linha, "%d,", &id);
        if (id != mapaID) continue;

        char *start = strchr(linha, '"');
        if (start) strcat(conteudoVer, start + 1);
        while (!strchr(conteudoVer, '"') && fgets(linha, sizeof(linha), f2))
        {
            strcat(conteudoVer, linha);
        }
        break;
    }
    fclose(f2);
    int i = 0;
    int px = 0, py = 0, pode = 0;

    for (i = 0; conteudoVer[i] != '\0'; i++)
    {

        if (px == novoX && py == novoY)
        {
            if (conteudoVer[i] != '#' && conteudoVer[i] != '_' && conteudoVer[i]!= ' ')
            {
                inimigo =  conteudoVer[i];//'O';
                encontrou = 1;
                printf("pode ver");
            }

            else if (conteudoVer[i] != '#' && conteudoVer[i] != '_' )
            {
                inimigo=  conteudoVer[i];
                pode = 1;
            }





            if (conteudoVer[i] == '>')
            {
                char linha[MAX];
                int cenas,maps,pXs,pYs;
                FILE *f = fopen("doors_out.csv", "r");

                while(fgets(linha, MAX, f))
                {
                    sscanf(linha,"%d,%d,%d,%d",&cenas,&maps,&pXs,&pYs);
                    if (cenas== cenaAtual && maps == mapaAtual+1)
                    {
                        mapaAtual++;
                        posX = pXs;
                        posY = pYs;

                        mapa(mapaAtual);

                    }


                }
                if (f)
                {
                    fclose(f);
                }

                return;
            }

            //retorna para mapa anterior
            if (conteudoVer[i] == '<')
            {
                char linha[MAX];
                int cenas,maps,pXs,pYs;
                FILE *f = fopen("doors_in.csv", "r");

                while(fgets(linha, MAX, f))
                {
                    sscanf(linha,"%d,%d,%d,%d",&cenas,&maps,&pXs,&pYs);
                    if (cenas== cenaAtual && maps == mapaAtual-1)
                    {
                        mapaAtual--;
                        posX = pXs;
                        posY = pYs;

                        mapa(mapaAtual);

                    }


                }
                if (f)
                {
                    fclose(f);
                }

                return;
            }

        }
        if (conteudoVer[i] == '\n')
        {
            py++;
            px = 0;
        }
        else px++;
    }


    if (pode)
    {
        posX = novoX;
        posY = novoY;
    }




    salvarJogo();
    mapa(mapaID);
}

void salvarJogo()
{

    FILE *f = fopen("saves.csv", "r");
    if (f == NULL)
    {
        // Se não existir ainda, cria o arquivo com cabeçalho
        f = fopen("saves.csv", "w");
        fprintf(f, "nome,cena,mapa,posiçãoX,posiçãoY\n");
        fclose(f);
        f = fopen("saves.csv", "r");
    }

    FILE *tempSave = fopen("tempSave.csv", "w");
    if (tempSave == NULL)
    {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(f);
        return;
    }

    fprintf(tempSave, "nome,cena,mapa,posiçãoX,posiçãoY\n");

    char linha[MAX], nomeLido[MAX];
    int cena, map, pX, pY;
    int encontrado = 0;

    // Ignora cabeçalho
    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f))
    {
        sscanf(linha, "%[^,],%d,%d,%d,%d", nomeLido, &cena, &map, &pX, &pY);
        if (strcmp(usuarioLogado, nomeLido) == 0)
        {
            fprintf(tempSave, "%s,%d,%d,%d,%d\n", usuarioLogado, cenaAtual, mapaAtual, posX, posY);
            encontrado = 1;
        }
        else
        {
            fprintf(tempSave, "%s,%d,%d,%d,%d\n", nomeLido, cena, map, pX, pY);
        }
    }

    fclose(f);
    fclose(tempSave);

    if (encontrado == 1)
    {
        remove("saves.csv");
        rename("tempSave.csv", "saves.csv");
    }
    else
    {
        // Adiciona nova linha no final
        FILE *append = fopen("saves.csv", "a");
        if (append != NULL)
        {
            fprintf(append, "%s,%d,%d,%d,%d\n", usuarioLogado, cenaAtual, mapaAtual, posX, posY);
            fclose(append);
        }
        remove("tempSave.csv");
    }

}

int verificarInimigo(char inimigo)
{
    FILE *f = fopen("personagens.csv", "r");
    if (!f) return 0;
    char linha[200];
    char nome[20];
    int mapa, px, py;
    fgets(linha, 200, f);
    while (fgets(linha, 200, f))
    {

        sscanf(linha, "%d,%[^,],%d,%d,%*d,%*d", &mapa,nome, &px, &py);
        if (nome[0] == inimigo)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int atacarInimigo(char inimigo)
{
    FILE *f = fopen("personagens.csv", "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!f || !temp) return;

    char linha[200], nome[20];
    int mapa, px, py, hp, forca;
    fgets(linha, 200, f);
    fprintf(temp, "%s", linha);

    while (fgets(linha, 200, f))
    {
        sscanf(linha, "%d,%[^,],%d,%d,%d,%d", &mapa, nome, &px, &py, &hp, &forca);
        if ( mapa == mapaAtual && nome[0] == inimigo)
        {
            hp -= 10;
            printf("Você atacou %s! HP restante: %d\n", nome, hp);
            Sleep(1500);
            if (hp > 0)
                fprintf(temp, "%d,%s,%d,%d,%d,%d\n", mapa, nome, px, py, hp, forca);
        }
        else
        {
            fprintf(temp, "%d,%s,%d,%d,%d,%d\n", mapa, nome, px, py, hp, forca);
        }
    }
    printf("%c",inimigo);
    fclose(f);
    fclose(temp);
    remove("personagens.csv");
    rename("temp.csv", "personagens.csv");
}
