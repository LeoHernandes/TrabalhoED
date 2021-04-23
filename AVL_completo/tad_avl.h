// TIPOS ESTRUTURADOS ***********************
typedef struct Tipo_Stats
{
    int nodos;              // Contagem dos nodos da árvore
    int rotacoes;           // Contagem das rotações feitas na indexação
    int altura;             // Altura total da árvore
    int comparacoes_index;  // Comparações feitas na indexação
    int comparacoes_search; // Comparações feitas na consulta
}Stats;

//*******************************************

typedef struct Tipo_LSE
{
    int info;               // ID do tweet
    struct Tipo_LSE* prox;  // Ponteiro para o próximo elemento da lsita
}LSE;

//*******************************************

typedef struct Tipo_Desc
{
    LSE *inicioLista;       // Aponta para o início da lista
    LSE *fimLista;          // Aponta para o fim da lista
}Descritor;

//*******************************************

typedef struct Tipo_AVL
{
    struct Tipo_AVL *esq;   // Ponteiro para subárvore esquerda
    struct Tipo_AVL *dir;   // Ponteiro para subárvore direita
    int FB;                 // Fator de balanceamento do nodo atual
    char *info;             // Palavra do nodo
    Descritor desc;         // Descritor da lista de incidência, apontando para o início e para o fim
}NodoAVL;

// OPERAÇÕES ********************************

Descritor inicializa_lista(Descritor desc);

Descritor insere_LSE(Descritor desc, int info);

NodoAVL* inicializa_arvore();

void imprime_formatado(NodoAVL* raiz, int nivel);

int altura (NodoAVL *nodo);

NodoAVL* insere_arvore(NodoAVL* raiz, char* palavra, int id, int* ok, Stats* stats);

NodoAVL* consulta_arvore(NodoAVL* raiz, char* palavra, Stats* stats);
