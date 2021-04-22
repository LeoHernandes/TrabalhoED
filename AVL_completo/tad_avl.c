#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

//*************************************************
//   OPERAÇÕES DA LISTA SIMPLESMENTE ENCADEADA
//*************************************************

LSE* inicializa_lista()
/* Inicializa o ponteiro para o início da lista */
{
    return NULL;
}

LSE* insere_inicioLSE(LSE* iniLista, int info)
/* Insere no início da lista os dados de um novo jogo.
 * Atualiza o ponteiro para o início da lista caso for possível inserir um elemento no início.
 * Se não houver mais espaço na memória, apenas devolve o início da lista */
{
    LSE* novo;

    novo = (LSE*)malloc(sizeof(LSE));

    if(novo != NULL)
    //se teve espaço na memória para alocar
    {
        novo->info = info;
        novo->prox = iniLista;
        iniLista = novo;
    }

    //retorna o elemento novo, mesmo que ele seja NULL, alertando sobre o erro
    //ou retorna o ponteiro para esse novo termo que é o mesmo que *iniLista
    return iniLista;
}

//*************************************************
//           OPERAÇÕES DA ÁRVORE AVL
//*************************************************

NodoAVL* inicializa_arvore()
/* Retorna NULL para inicializar o ponteiro par aa raiz da árvore */
{
    return NULL;
}

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

NodoAVL* desbalanco_dir(NodoAVL* nodo, int* ok, Stats* stats)
/* Faz o balanceamento do nodo dado quando seu fator
 * de desbalanceamento é -2.
 * Efetua uma rotação simples ou dupla à direita. */
{
    NodoAVL* aux;

    aux = nodo->dir;

    if(aux->FB == -1) //se o nodo direito tem FB = -1;
    {
        nodo =  rotacao_esquerda_S(nodo);
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
        strcpy(raiz->info, palavra);            //insere a palavra no nodo
        raiz->lista_id = inicializa_lista();
        raiz->lista_id = insere_inicioLSE(raiz->lista_id, id); //insere o id na lista de ocorrências
        *ok = 1;
        stats->nodos++;    //aumenta a contagem de nodos na arvore
    }
    else if(strcmp(palavra, raiz->info) < 0)
    //se o novo nodo é lexicograficamente menor que o nodo atual
    {
        raiz = insere_arvore(raiz->esq, palavra, id, ok, stats); // vai para o nodo esquerdo
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
        raiz = insere_arvore(raiz->dir, palavra, id, ok, stats); //vai para o nodo direito
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
        raiz->lista_id = insere_inicioLSE(raiz->lista_id, id);
    }

    return raiz;
}
