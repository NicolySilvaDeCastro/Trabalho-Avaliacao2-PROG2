#include <stdio.h>
#include <stdlib.h>

/* ---------- Estrutura do nó ---------- */
typedef struct No {
    int valor;
    struct No *esq;
    struct No *dir;
} No;

/* ---------- Criação de nó ---------- */
No* criarNo(int valor) {
    No *novo = (No*) malloc(sizeof(No));
    if (!novo) {
        fprintf(stderr, "Erro de alocação\n");
        exit(1);
    }
    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

/* ---------- Inserção ---------- */
No* inserir(No *raiz, int valor) {
    if (raiz == NULL)
        return criarNo(valor);

    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = inserir(raiz->dir, valor);
    /* valor igual: ignora (sem duplicatas) */

    return raiz;
}

/* ---------- Busca ---------- */
No* buscar(No *raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor)
        return raiz;

    if (valor < raiz->valor)
        return buscar(raiz->esq, valor);
    return buscar(raiz->dir, valor);
}

/* ---------- Menor valor da subárvore ---------- */
No* menor_no(No *raiz) {
    while (raiz && raiz->esq != NULL)
        raiz = raiz->esq;
    return raiz;
}

/* ---------- Remoção ---------- */
No* remover_no(No *raiz, int valor) {
    if (raiz == NULL) return NULL;

    if (valor < raiz->valor) {
        raiz->esq = remover_no(raiz->esq, valor);
    } else if (valor > raiz->valor) {
        raiz->dir = remover_no(raiz->dir, valor);
    } else {
        /* Caso 1: sem filho ou 1 filho */
        if (raiz->esq == NULL) {
            No *temp = raiz->dir;
            free(raiz);
            return temp;
        }
        if (raiz->dir == NULL) {
            No *temp = raiz->esq;
            free(raiz);
            return temp;
        }
        /* Caso 2: dois filhos -> sucessor in-order (menor da direita) */
        No *sucessor = menor_no(raiz->dir);
        raiz->valor = sucessor->valor;
        raiz->dir = remover_no(raiz->dir, sucessor->valor);
    }
    return raiz;
}

/* ---------- Percursos ---------- */
void emOrdem(No *raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    printf("%d ", raiz->valor);
    emOrdem(raiz->dir);
}

void preOrdem(No *raiz) {
    if (raiz == NULL) return;
    printf("%d ", raiz->valor);
    preOrdem(raiz->esq);
    preOrdem(raiz->dir);
}

void posOrdem(No *raiz) {
    if (raiz == NULL) return;
    posOrdem(raiz->esq);
    posOrdem(raiz->dir);
    printf("%d ", raiz->valor);
}

/* ---------- Altura ---------- */
int altura(No *raiz) {
    if (raiz == NULL) return -1;
    int he = altura(raiz->esq);
    int hd = altura(raiz->dir);
    return (he > hd ? he : hd) + 1;
}

/* ---------- Contar nós ---------- */
int contarNos(No *raiz) {
    if (raiz == NULL) return 0;
    return 1 + contarNos(raiz->esq) + contarNos(raiz->dir);
}

/* ---------- Liberar memória ---------- */
void liberar(No *raiz) {
    if (raiz == NULL) return;
    liberar(raiz->esq);
    liberar(raiz->dir);
    free(raiz);
}

/* ---------- Visualização simples (deitada) ---------- */
void imprimir_arvore(No *raiz, int nivel) {
    if (raiz == NULL) return;
    imprimir_arvore(raiz->dir, nivel + 1);
    for (int i = 0; i < nivel; i++) printf("    ");
    printf("%d\n", raiz->valor);
    imprimir_arvore(raiz->esq, nivel + 1);
}

/* ---------- main ---------- */
int main(void) {
    No *raiz = NULL;
    int valores[] = {50, 30, 70, 20, 40, 60, 80, 100, 101, 200, 300, 400, 500, 700, 1, 800, 900, 1000};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++)
        raiz = inserir(raiz, valores[i]);

    printf("=== Árvore (visualização deitada) ===\n");
    imprimir_arvore(raiz, 0);

    printf("\nEm ordem:  ");
    emOrdem(raiz);
    printf("\nPré ordem: ");
    preOrdem(raiz);
    printf("\nPós ordem: ");
    posOrdem(raiz);

    printf("\n\nAltura: %d", altura(raiz));
    printf("\nNós:    %d\n", contarNos(raiz));

    /* Busca */
    int alvo = 40;
    No *achado = buscar(raiz, alvo);
    printf("\nBuscar %d: %s\n", alvo, achado ? "encontrado" : "não encontrado");

    /* Remoção */
    printf("\nRemovendo 30...\n");
    raiz = remover_no(raiz, 30);
    printf("Em ordem: ");
    emOrdem(raiz);
    printf("\n");

    liberar(raiz);
    return 0;
}