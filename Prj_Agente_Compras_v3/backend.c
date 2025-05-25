#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#define MAX_PRODUTO_ARQUIVO 100
char usuario[50];

void ver_estoque()
{
  //ira mostra todos os produtos que serao filtrados pelo setor
   info_estoqueVoltar();

  /*perguntar para o usuario se ele quer filtrar por nome,setor ou validade
    perguntar para o usuario se ele quer adicionar algo ou remover algo
    a partirdas informações apresentadas.(não precisa implementar agora).
  */


}
void modificar_estoque()
{
       FILE *fpont;
   const int lim = 50;
   char linhas[MAX_PRODUTO_ARQUIVO + 3];
   char nomeProduto[50];
   int novaQuantidade;
   int qtd_guardada;
   char setor_guardado[lim];
   char nome_guardado[lim];

    int dia_guardado, mes_guardado, ano_guardado;
   fpont = fopen("estoque.txt", "r");
   if (fpont == NULL)
   {
       perror("Nao foi possivel abrir o arquivo de estoque");
       exit(1);
   }

   printf("\tEstoque\n\n");

   while (fgets(linhas, sizeof(linhas), fpont) != NULL)
   {


       if (sscanf(linhas, "%49[^:]:%49[^:]:%d:%d:%d:%d",setor_guardado, nome_guardado,&qtd_guardada, &dia_guardado, &mes_guardado, &ano_guardado) == 6)
       {

           printf("Setor: %s    Nome: %s    Quantidade: %d\n\n",setor_guardado, nome_guardado, qtd_guardada);

       }



   }
   fclose(fpont);

   printf("      Escolha o produto que sera alterado a quantidade :\n");

    printf("\nNome: ");
    scanf("%s",&nomeProduto);

    printf("\n ");

    printf("\nQuantidade: ");
    scanf(" %d",&novaQuantidade);


    alterar_quantidade(nomeProduto,novaQuantidade);



}
void add_produto()
{
    char nome[50];
    int qtd;
    int dia;
    int mes;
    int ano;
    char setor[50];
    //adiciona produtos
    printf("Dados do produto\n\n");

    printf("Informe o setor do produto: ");
    fgets(setor,sizeof(setor),stdin);
    setor[strcspn(setor, "\n")] = 0;

    printf("Informe o nome do produto: ");
    fgets(nome,sizeof(nome),stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Informe a quantidade do produto: ");
    scanf("%d",&qtd);

    printf("Informe a data de validade do produto(D M A): ");
    scanf("%d %d %d",&dia,&mes,&ano);

    add_estoque(setor,nome,qtd,dia,mes,ano);


}
void del_produto()
 {

   FILE *f= fopen("estoque.txt", "r");
   const int lim = 50;
   char linhas[MAX_PRODUTO_ARQUIVO + 3];
   int count=0;

   while(fgets(linhas, sizeof(linhas), f) != NULL)
   {
       count++;


   }
   fclose(f);
  if(count == 0)
  {

      printf("Estoque Vazio! Escolha Adicionar produto");
      fclose(f);


      voltar();
      return;

  }

    info_estoque();
    char nome[50];
    printf("\nDigite o Nome do produto que deseja remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    del_estoque(nome);


     voltar();
  }


void criacao_lista()
{
    //adiciona os produtos em um documento texto(em forma de lista de compras);
    criar_lista_compras();
    pausar(1000);
}
void voltar()
{

    char ch;

    printf("\n\nPrecione qualquer tecla para retornar ao menu\n");
    ch = getch();
    menu(usuario);
}
