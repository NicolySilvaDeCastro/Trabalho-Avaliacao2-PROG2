
#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    int altura;
    struct No *esquerda;
    struct No *direita;
} No;

// Retorna a altura de um no
int altura(No *no) {
    return no == NULL ? 0 : no->altura;
}

// Retorna o maior entre dois inteiros
int maximo(int a, int b) {
    return a > b ? a : b;
}

// Atualiza a altura do no
void atualizarAltura(No *no) {
    no->altura = 1 + maximo(
        altura(no->esquerda),
        altura(no->direita)
    );
}

// Calcula o fator de balanceamento
int fator_balanceamento(No *no) {
    if (no == NULL) {
        return 0;
    }

    return altura(no->esquerda)
         - altura(no->direita);
}

// Rotacao simples para a direita
No* rotacao_a_direita(No *y) {
    No *x = y->esquerda;
    No *t2 = x->direita;

    x->direita = y;
    y->esquerda = t2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

// Rotacao simples para a esquerda
No* rotacao_a_esquerda(No *x) {
    No *y = x->direita;
    No *t2 = y->esquerda;

    y->esquerda = x;
    x->direita = t2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

// Cria um novo no
No* criarNo(int valor) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        fprintf(stderr, "Erro de memoria!\n");
        exit(EXIT_FAILURE);
    }

    novo->valor = valor;
    novo->altura = 1;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

// Insere e balanceia a arvore
No* inserir(No *raiz, int valor) {
    if (raiz == NULL) {
        return criarNo(valor);
    }

    if (valor < raiz->valor) {
        raiz->esquerda =
            inserir(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita =
            inserir(raiz->direita, valor);
    } else {
        return raiz; // Ignora valores repetidos
    }

    atualizarAltura(raiz);

    int fb = fator_balanceamento(raiz);

    // Caso esquerda-esquerda (LL)
    if (fb > 1 &&
        valor < raiz->esquerda->valor) {
        return rotacao_a_direita(raiz);
    }

    // Caso direita-direita (RR)
    if (fb < -1 &&
        valor > raiz->direita->valor) {
        return rotacao_a_esquerda(raiz);
    }

    // Caso esquerda-direita (LR)
    if (fb > 1 &&
        valor > raiz->esquerda->valor) {
        raiz->esquerda =
            rotacao_a_esquerda(raiz->esquerda);
        return rotacao_a_direita(raiz);
    }

    // Caso direita-esquerda (RL)
    if (fb < -1 &&
        valor < raiz->direita->valor) {
        raiz->direita =
            rotacao_a_direita(raiz->direita);
        return rotacao_a_esquerda(raiz);
    }

    return raiz;
}

// Pesquisa um valor
No* buscar(No *raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }

    if (valor < raiz->valor) {
        return buscar(raiz->esquerda, valor);
    }

    return buscar(raiz->direita, valor);
}

// Exibe em ordem crescente
void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        emOrdem(raiz->direita);
    }
}

// Encontra o menor no de uma subarvore
No* menor_no(No *raiz) {
    No *atual = raiz;

    while (atual != NULL &&
           atual->esquerda != NULL) {
        atual = atual->esquerda;
    }

    return atual;
}

// Remove um valor e rebalanceia a arvore
No* remover_no(No *raiz, int valor) {
    if (raiz == NULL) {
        return NULL;
    }

    if (valor < raiz->valor) {
        raiz->esquerda =
            remover_no(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita =
            remover_no(raiz->direita, valor);
    } else {
        // No com zero ou um filho
        if (raiz->esquerda == NULL ||
            raiz->direita == NULL) {

            No *filho = raiz->esquerda != NULL
                      ? raiz->esquerda
                      : raiz->direita;

            free(raiz);
            return filho;
        }

        // No com dois filhos:
        // substitui pelo sucessor em ordem
        No *sucessor = menor_no(raiz->direita);
        raiz->valor = sucessor->valor;

        raiz->direita =
            remover_no(raiz->direita, sucessor->valor);
    }

    atualizarAltura(raiz);

    int fb = fator_balanceamento(raiz);

    // Esquerda-esquerda
    if (fb > 1 &&
        fator_balanceamento(raiz->esquerda) >= 0) {
        return rotacao_a_direita(raiz);
    }

    // Esquerda-direita
    if (fb > 1 &&
        fator_balanceamento(raiz->esquerda) < 0) {
        raiz->esquerda =
            rotacao_a_esquerda(raiz->esquerda);
        return rotacao_a_direita(raiz);
    }

    // Direita-direita
    if (fb < -1 &&
        fator_balanceamento(raiz->direita) <= 0) {
        return rotacao_a_esquerda(raiz);
    }

    // Direita-esquerda
    if (fb < -1 &&
        fator_balanceamento(raiz->direita) > 0) {
        raiz->direita =
            rotacao_a_direita(raiz->direita);
        return rotacao_a_esquerda(raiz);
    }

    return raiz;
}

// Mostra os valores e suas alturas
void imprimir_detalhes(No *raiz) {
    if (raiz != NULL) {
        imprimir_detalhes(raiz->esquerda);

        printf("Valor: %d | Altura: %d | Fator: %d\n",
               raiz->valor,
               raiz->altura,
               fator_balanceamento(raiz));

        imprimir_detalhes(raiz->direita);
    }
}

// Libera a memoria
void liberar(No *raiz) {
    if (raiz != NULL) {
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}

int main(void) {
    No *raiz = NULL;
    int opcao, valor;

    do {
        printf("\n===== ARVORE AVL =====\n");
        printf("1 - Inserir\n");
        printf("2 - Mostrar em ordem\n");
        printf("3 - Buscar\n");
        printf("4 - Remover\n");
        printf("5 - Mostrar alturas e fatores\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            liberar(raiz);
            return EXIT_FAILURE;
        }

        switch (opcao) {
            case 1:
                printf("Valor: ");
                if (scanf("%d", &valor) != 1) {
                    liberar(raiz);
                    return EXIT_FAILURE;
                }

                raiz = inserir(raiz, valor);
                break;

            case 2:
                printf("Em ordem: ");
                emOrdem(raiz);
                printf("\n");
                break;

            case 3:
                printf("Valor para buscar: ");
                if (scanf("%d", &valor) != 1) {
                    liberar(raiz);
                    return EXIT_FAILURE;
                }

                if (buscar(raiz, valor) != NULL) {
                    printf("Valor encontrado!\n");
                } else {
                    printf("Valor nao encontrado.\n");
                }
                break;

            case 4:
                printf("Valor para remover: ");
                if (scanf("%d", &valor) != 1) {
                    liberar(raiz);
                    return EXIT_FAILURE;
                }

                if (buscar(raiz, valor) == NULL) {
                    printf("Valor nao encontrado!\n");
                } else {
                    raiz = remover_no(raiz, valor);
                    printf("Valor removido!\n");
                }
                break;

            case 5:
                imprimir_detalhes(raiz);
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    liberar(raiz);
    return EXIT_SUCCESS;
}