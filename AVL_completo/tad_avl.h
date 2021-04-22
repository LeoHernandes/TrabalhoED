// TIPOS ESTRUTURADOS ***********************
typedef struct Tipo_Stats
{
    int nodos;
    int rotacoes;
    int altura;
    int comparacoes_index;
    int comparacoes_search;
}Stats;

typedef struct Tipo_LSE
{
    int info;               // ID do tweet
    struct Tipo_LSE* prox;  // Ponteiro para o próximo elemento da lsita
}LSE;

typedef struct Tipo_AVL
{
    struct Tipo_AVL *esq;   // Ponteiro para subárvore esquerda
    struct Tipo_AVL *dir;   // Ponteiro para subárvore direita
    int FB;                 // Fator de balanceamento do nodo atual
    char *info;             // Palavra do nodo
    LSE *lista_id;          // Lista de incidência da palavra com os ID dos tweets em que ela aparece
}NodoAVL;

// OPERAÇÕES ********************************

LSE* inicializa_lista();

LSE* insere_inicioLSE(LSE* iniLista, int info);

NodoAVL* inicializa_arvore();

void imprime_formatado(NodoAVL* raiz, int nivel);

int altura (NodoAVL *nodo);

NodoAVL* insere_arvore(NodoAVL* raiz, char* palavra, int id, int* ok, Stats* stats);
