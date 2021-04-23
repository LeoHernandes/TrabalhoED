#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "avl.h"

void converte_minuscula (char *s)
{
    int i;
    for (i=0; i<strlen(s); i++)
        s[i]=tolower(s[i]);
}

int main (int argc, char *argv[])
{

    setlocale(LC_ALL, ""); //para imprimir corretamente na tela os caracteres acentuados
    FILE *entrada;
    FILE *saida;
    FILE *consulta;
    NodoAVL *raiz;
    NodoAVL *nodoProcurado;
    LSE *listaID;
    Stats estatisticas = {0}; //variavel que guarda as estatistas da arvore

    char *palavra, linha[300], *id; // linhas a serem lidas do arquivo
    char separador[] = {" 0123456789,.&*%\?!;/-'@\"$#=~><()][}{:\n\t_"}; //caracteres separadores para as palavras
    int id_num, ok = 0;


    /*if(argc != 4)  //testa se o numero de parametros esperado está correto (deve ser 3): nome do programa (argv[0]), nome do arq de entrada(argv[1]), nome do arq de consulta(argv[2]), nome do arq de saida(argv[3])
    {
        printf ("Número incorreto de parâmetros.\n Para chamar o programa digite: %s <arq_entrada> <arq_consulta> <arq_saida>\n",argv[0]);
        return 1;
    }*/

    if((entrada = fopen(argv[1], "r")) == NULL)   // testa se consegue abrir o arquivo de entrada
    {
        printf("*** Erro ao abrir arquivo de entrada ***\n");
        return 1;
    }

    raiz = inicializa_arvore();

    while(fgets(linha, 1000, entrada)) //l� cada linha do arquivo texto
    {
        id = strtok (linha, ";"); //pega o id do tweet
        id_num = atoi(id); //converte o id para num�rico (vai ser necess�rio para inserir o id tweet na lista de ocorr�ncias)
        palavra = strtok (NULL, separador); //pega a 1a palavra do tweet

        while (palavra != NULL) //enquanto encontrar palavras no tweet
        {
            converte_minuscula(palavra); //converte a palavra para min�sculo
            raiz = insere_arvore(raiz, palavra, id_num, &ok, &estatisticas); //indexa a palavra na árvore
            palavra = strtok (NULL, separador); //pega a pr�xima palavra do tweet
        }

    }
    fclose(entrada); //fecha os arquivos

    estatisticas.altura = altura(raiz); //calcula altura da arvore resultante

    if((consulta = fopen(argv[2], "r")) == NULL)
    {
        printf("*** Erro ao abrir o arquivo de consulta! ***\n");
        return 1;
    }

    if ((saida = fopen(argv[3], "w")) == NULL)   // testa se consegue criar o arquivo de saída
    {
        printf("*** Erro ao criar arquivo de saida! ***\n");
        return 1;
    }

    while(fgets(linha, 1000, consulta)) //l� cada linha do arquivo de consulta
    {

        palavra = strtok (linha, "\n"); //retira o '\n' do final da linha
        nodoProcurado = consulta_arvore(raiz, palavra, &estatisticas);

        if(nodoProcurado == NULL)   //se o nodo não foi encontrado
            fprintf(saida, "Consulta: %-25s Palavra não encontrada\n", palavra);
        else                        //se o nodo foi encontrado
        {
            listaID = nodoProcurado->lista_id;
            fprintf(saida, "Consulta: %-25s Palavra encontrada nos tweets", palavra);
            while(listaID != NULL)
            {
                fprintf(saida, " %d,", listaID->info); //escreve as incidências do tweet
                listaID = listaID->prox;
            }
            fprintf(saida, "\n");
        }
    }

    fclose(consulta);

    fprintf(saida, "\n********** Estatísticas da Indexação **************\n");
    fprintf(saida, "nodos = %d\n", estatisticas.nodos);
    fprintf(saida, "comparações = %d\n", estatisticas.comparacoes_index);
    fprintf(saida, "rotações = %d\n", estatisticas.rotacoes);
    fprintf(saida, "altura da árvore = %d\n\n", estatisticas.altura);
    fprintf(saida, "********** Estatísticas das Consultas **************\n");
    fprintf(saida, "comparações = %d", estatisticas.comparacoes_search);

    fclose(saida);

    return 0; //retorna 0 para indicar que foi bem sucedido
}
