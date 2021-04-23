#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

//*************************************************
//   OPERAÇÕES DA LISTA SIMPLESMENTE ENCADEADA
//*************************************************

Descritor inicializa_lista(Descritor desc)
/* Inicializa o ponteiro para o início da lista */
{
    desc.fimLista = NULL;
    desc.inicioLista = NULL;

    return desc;
}

//********************************************************************
//********************************************************************

Descritor insere_LSE(Descritor desc, int info)
/* Insere no início da lista os dados de um novo jogo.
 * Atualiza o descritor da lista caso for possível inserir um elemento no final.
 * Se não houver mais espaço na memória, apenas devolve o descritor da lista */
{
    LSE* novo;

    novo = (LSE*)malloc(sizeof(LSE));

    if(novo != NULL)
    //se teve espaço na memória para alocar
    {
        novo->info = info;      //guarda a informação

        if(desc.inicioLista == NULL)    //se a lista está vazia
        {
            desc.inicioLista = novo;    //novo ID é o início e fim da lista
            desc.fimLista = novo;
        }
        else                            //senão
        {
            desc.fimLista->prox = novo; //fim da lista aponta para o novo termo
            desc.fimLista = novo;       //novo termo se torna o fim da lista
        }

        novo->prox = NULL;
    }

    //retorna o descritor da lista
    return desc;
}

//********************************************************************
//********************************************************************

int consulta_lista(Descritor desc, int info)
/* Verifica se um dado ID está na lista.
 * Devolve 1 se ele estiver.
 * Devolve 0 se não estiver. */
{
    while(desc.inicioLista != NULL) //enquanto não chegou no final da lista
    {
        if(desc.inicioLista->info == info) //se encontrar o elemento
            return 1;              //retorna 1
        desc.inicioLista = desc.inicioLista->prox;
    }

    return 0; //se não encontrou o elemento, retorna 0
}

//*************************************************
//           OPERAÇÕES DA ÁRVORE AVL
//*************************************************

NodoAVL* inicializa_arvore()
/* Retorna NULL para inicializar o ponteiro par aa raiz da árvore */
{
    return NULL;
}

//********************************************************************
//********************************************************************

void imprime_formatado(NodoAVL* raiz, int nivel)
/* Imprime a árvore formatadamente com identação para identificar os diferentes níveis dos nodos */
{
    int i;
    if(raiz != NULL)
    {
        for(i = 0; i < nivel; i++)
        {
            printf("=");
        }
        printf("%s\n", raiz->info);

        imprime_formatado(raiz->esq, nivel + 1);
        imprime_formatado(raiz->dir, nivel + 1);
    }
}

//********************************************************************
//********************************************************************

int altura(NodoAVL *nodo)
/* Calcula a altura de uma árvore ou subárvore dada a sua raíz. */
{
    int Alt_Esq, Alt_Dir;

    if (nodo == NULL)
        return 0;
    else
    {
        Alt_Esq = altura(nodo->esq);
        Alt_Dir = altura(nodo->dir);
        if (Alt_Esq > Alt_Dir)
            return (1 + Alt_Esq); //devolve a altura da subárvore mais alta
        else
            return (1 + Alt_Dir);
    }
}

//********************************************************************
//********************************************************************

NodoAVL* rotacao_direita_S(NodoAVL* nodo)
/* Faz uma rotação simples à direita em torno do nodo dado */
{
    NodoAVL* aux;

    aux = nodo->esq;
    nodo->esq = aux->dir;
    aux->dir = nodo;
    nodo->FB = 0;
    nodo = aux;

    return nodo;
}

//********************************************************************
//********************************************************************

NodoAVL* rotacao_esquerda_S(NodoAVL* nodo)
/* Faz uma rotação simples à esquerda em torno do nodo dado */
{
    NodoAVL* aux;

    aux = nodo->dir;
    nodo->dir = aux->esq;
    aux->esq = nodo;
    nodo->FB = 0;
    nodo = aux;

    return nodo;
}

//********************************************************************
//********************************************************************

NodoAVL* rotacao_direita_D(NodoAVL* nodo)
/* Faz uma rotação dupla à direita em torno do nodo dado */
{
    NodoAVL *auxFilho, *auxNeto;

    auxFilho = nodo->esq;
    auxNeto = auxFilho->dir;
    auxFilho->dir = auxNeto->esq;
    auxNeto->esq = auxFilho;
    nodo->esq = auxNeto->dir;
    auxNeto->dir = nodo;

    if(auxNeto->FB == 1)
        nodo->FB = -1;
    else
        nodo->FB = 0;

    if(auxNeto->FB == -1)
        auxFilho->FB = 1;
    else
        auxFilho->FB = 0;

    nodo = auxNeto;
    return nodo;
}

//********************************************************************
//********************************************************************

NodoAVL* rotacao_esquerda_D(NodoAVL* nodo)
/* Faz uma rotação dupla à esquerda em torno do nodo dado */
{
    NodoAVL *auxFilho, *auxNeto;

    auxFilho = nodo->dir;
    auxNeto = auxFilho->esq;
    auxFilho->esq = auxNeto->dir;
    auxNeto->dir = auxFilho;
    nodo->dir = auxNeto->esq;
    auxNeto->esq = nodo;

    if(auxNeto->FB == -1)
        nodo->FB = 1;
    else
        nodo->FB = 0;

    if(auxNeto->FB == 1)
        auxFilho->FB = -1;
    else
        auxFilho->FB = 0;

    nodo = auxNeto;
    return nodo;
}

//********************************************************************
//********************************************************************

NodoAVL* desbalanco_esq(NodoAVL* nodo, int* ok, Stats* stats)
/* Faz o balanceamento do nodo dado quando seu fator
 * de desbalanceamento é 2.
 * Efetua uma rotação simples ou dupla à direita. */
{
    NodoAVL* aux;

    aux = nodo->esq;

    if(aux->FB == 1)  //se o nodo esquerdo tem FB = 1;
    {
        nodo = rotacao_direita_S(nodo);
        stats->rotacoes++;      //soma uma rotação
    }
    else              //se o nodo esquerdo tem FB = -1;
    {
        nodo = rotacao_direita_D(nodo);
        stats->rotacoes += 2;  //soma duas rotações
    }

    nodo->FB = 0;     //sinaliza que o nodo está agora balanceado
    *ok = 0;          //avisa para a função insere_arvore que está feito os balanceamentos
    return nodo;
}

//********************************************************************
//********************************************************************

NodoAVL* desbalanco_dir(NodoAVL* nodo, int* ok, Stats* stats)
/* Faz o balanceamento do nodo dado quando seu fator
 * de desbalanceamento é -2.
 * Efetua uma rotação simples ou dupla à direita. */
{
    NodoAVL* aux;

    aux = nodo->dir;

    if(aux->FB == -1) //se o nodo direito tem FB = -1;
    {
        nodo = rotacao_esquerda_S(nodo);
        stats->rotacoes++;
    }
    else              //se o nodo direito tem FB = 1;
    {
        nodo = rotacao_esquerda_D(nodo);
        stats->rotacoes += 2;
    }

    nodo->FB = 0;     //sinaliza que o nodo está agora balanceado
    *ok = 0;          //avisa para a função insere_arvore que está feito os balanceamentos
    return nodo;
}

//********************************************************************
//********************************************************************

NodoAVL* insere_arvore(NodoAVL* raiz, char* palavra, int id, int* ok, Stats* stats)
/* Insere um elemento na árvore AVL.
 * Palavras lexicograficamente menores vão para esquerda.
 * As maiores vão para a direita.
 * Não há elementos repetidos, são adicionados "IDs" de ocorrências nos nodos apenas */
{
    if(raiz == NULL)
    //se chegamos num nodo folha ou a árvore é vazia, insere o novo nodo
    {
        raiz = (NodoAVL*)malloc(sizeof(NodoAVL));
        raiz->dir = NULL;
        raiz->esq = NULL;
        raiz->FB = 0;
        raiz->info = (char*)malloc((strlen(palavra) + 1) * sizeof(char)); //armazena espaço na memória para guardar a palavra
        strcpy(raiz->info, palavra);            //insere a palavra no nodo
        raiz->desc = inicializa_lista(raiz->desc);
        raiz->desc = insere_LSE(raiz->desc, id); //insere o id na lista de ocorrências
        *ok = 1;
        stats->nodos++;    //aumenta a contagem de nodos na arvore
    }
    else if(strcmp(palavra, raiz->info) < 0)
    //se o novo nodo é lexicograficamente menor que o nodo atual
    {
        stats->comparacoes_index++;
        raiz->esq = insere_arvore(raiz->esq, palavra, id, ok, stats); // vai para o nodo esquerdo
        if(*ok)
        {
            switch(raiz->FB)
            {
                case -1:
                    raiz->FB = 0;
                    *ok = 0;
                    break;
                case 0:
                    raiz->FB = 1;
                    break;
                case 1:
                    raiz = desbalanco_esq(raiz, ok, stats);
                    break;
            }
        }
    }
    else if(strcmp(palavra, raiz->info) > 0)
    //se o novo nodo é lexicograficamente maior que o nodo atual
    {
        stats->comparacoes_index++;
        raiz->dir = insere_arvore(raiz->dir, palavra, id, ok, stats); //vai para o nodo direito
        if(*ok)
        {
            switch(raiz->FB)
            {
                case 1:
                    raiz->FB = 0;
                    *ok = 0;
                    break;
                case 0:
                    raiz->FB = -1;
                    break;
                case -1:
                    raiz = desbalanco_dir(raiz, ok, stats);
                    break;
            }
        }
    }
    else
    //se a palavra já existe na árvore
    {
        stats->comparacoes_index++;
        if(!consulta_lista(raiz->desc, id)) //se o ID não existe ainda na lista de incidência
            raiz->desc = insere_LSE(raiz->desc, id);
    }

    return raiz;
}

//********************************************************************
//********************************************************************

NodoAVL* consulta_arvore(NodoAVL* raiz, char* palavra, Stats* stats)
/* Busca na árvore a palavra dada, retornando um ponteiro para o nodo encontrado.
 * Se não existir a palavra, retorna NULL */
{
    if(raiz != NULL)  //se a árvore não está vazia
    {
        stats->comparacoes_search++; //aumenta o número de comparações feitas nas consultas

        if(strcmp(palavra, raiz->info) < 0) //se a palavra é lexicograficamente menor que o nodo atual
        {
            return(consulta_arvore(raiz->esq, palavra, stats)); //vai para o nodo esquerdo
        }
        else if(strcmp(palavra, raiz->info) > 0) //se a palavra é lexicograficamente maior que o nodo atual
        {
            return(consulta_arvore(raiz->dir, palavra, stats)); //vai para o nodo direito
        }
        else        //se a palavra foi encontrada, retorna o ponteiro para esse nodo;
            return raiz;
    }
    else        //se não encontrou a palavra, retorna NULL
        return NULL;
}
