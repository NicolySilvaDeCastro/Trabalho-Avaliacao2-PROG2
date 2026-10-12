

#include <stdio.h>
#include <stdlib.h>

/***Definição da struct No***/
typedef struct No {
    int valor;
    int altura;
    struct No *esquerda;
    struct No *direita;
} No;
/***Códigos auxiliares para a função insrir_no***/
No* criar_no(int valor) {
    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        return NULL;
    }

    novo->valor = valor;
    novo->altura = 1;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

int altura_no(No * no) {
    if (no == NULL) {
        return 0;
    }
    return no->altura;
}

int maior_valor_altura(int a, int b) {
    if (a > b) {
        return a;
    }

    return b;
}


void atualizar_altura_no(No *no) {
    if (no != NULL) {
        no->altura = 1 + maior_valor_altura(        /*h(n)=1+max(h(ne),h(nd))*/
            altura_no(no->esquerda),
            altura_no(no->direita)
        );
    }
}

int fator_balanceamento/*indica a diferença entre a altura da subárvore esquerda e a da direita*/(No *no) {
    if (no == NULL) {
        return 0;
    }

    return altura_no(no->esquerda) - altura_no(no->direita); /*FB(n)=h(ne)−h(nd)*/
}

No* rotacao_a_direita(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    atualizar_altura_no(y);
    atualizar_altura_no(x);

    return x;
}


No* rotacao_a_esquerda(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    x->direita = T2;
    y->esquerda = x;

    atualizar_altura_no(x);
    atualizar_altura_no(y);

    return y;
}
/***Usáveis(não auxiliares)***/
No* inserir_no(No* raiz, int valor) {
    if (raiz == NULL) {
        return criar_no(valor);
    }

    if (valor < raiz->valor) {
        raiz->esquerda = inserir_no(raiz->esquerda, valor);
    }
    else if (valor > raiz->valor) {
        raiz->direita = inserir_no(raiz->direita, valor);
    }
    else {
        return raiz;
    }

    atualizar_altura_no(raiz);


    int fb = fator_balanceamento(raiz);


    if (fb > 1 && valor < raiz->esquerda->valor) {
        return rotacao_a_direita(raiz);
    }


    if (fb < -1 && valor > raiz->direita->valor) {
        return rotacao_a_esquerda(raiz);
    }


    if (fb > 1 && valor > raiz->esquerda->valor) {
        raiz->esquerda = rotacao_a_esquerda(raiz->esquerda);
        return rotacao_a_direita(raiz);
    }


    if (fb < -1 && valor < raiz->direita->valor) {
        raiz->direita = rotacao_a_direita(raiz->direita);
        return rotacao_a_esquerda(raiz);
    }

    return raiz;
}
/*função para printar a árvore(em forma de árvore)*/
void imprimir_arvore(No *raiz, int nivel) {
    if (raiz == NULL) {
        return;
    }

    imprimir_arvore(raiz->direita, nivel + 1);

    for (int i = 0; i < nivel; i++) {
        printf("    ");
    }

    printf("%d\n", raiz->valor);

    imprimir_arvore(raiz->esquerda, nivel + 1);
}
/*printa a árvore(em forma de ordem)*/
void impressao_em_ordem(No *raiz) {
    if (raiz == NULL) {
        return;
    }

    impressao_em_ordem(raiz->esquerda);
    printf("%d ", raiz->valor);
    impressao_em_ordem(raiz->direita);
    }



/*printa especificações(detalhes) de cada nó da arvore */
void imprimir_detalhes(No *raiz) {
    if (raiz == NULL) {
        return;
    }

    printf("Valor: %d | Altura: %d | FB: %d\n",
           raiz->valor,
           raiz->altura,
           fator_balanceamento(raiz));

    imprimir_detalhes(raiz->esquerda);
    imprimir_detalhes(raiz->direita);
}
/*auxiliar de remover_no |    */
/*                       |    */
/*                       v    */
No* menor_no(No *raiz) {
    No *atual = raiz;

    while (atual != NULL && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }

    return atual;
}
/*função que remove um nó da árvore*/

No* remover_no(No *raiz, int valor) {
    if (raiz == NULL) {
        return NULL;
    }


    if (valor < raiz->valor) {
        raiz->esquerda = remover_no(raiz->esquerda, valor);
    }
    else if (valor > raiz->valor) {
        raiz->direita = remover_no(raiz->direita, valor);
    }
    else {

        if (raiz->esquerda == NULL) {
            No *temp = raiz->direita;
            free(raiz);
            return temp;
        }


        if (raiz->direita == NULL) {
            No *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }


        No *temp = menor_no(raiz->direita);
        raiz->valor = temp->valor;
        raiz->direita = remover_no(raiz->direita, temp->valor);
    }


    atualizar_altura_no(raiz);


    int fb = fator_balanceamento(raiz);


    if (fb > 1 && fator_balanceamento(raiz->esquerda) >= 0) {
        return rotacao_a_direita(raiz);
    }


    if (fb > 1 && fator_balanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacao_a_esquerda(raiz->esquerda);
        return rotacao_a_direita(raiz);
    }


    if (fb < -1 && fator_balanceamento(raiz->direita) <= 0) {
        return rotacao_a_esquerda(raiz);
    }


    if (fb < -1 && fator_balanceamento(raiz->direita) > 0) {
        raiz->direita = rotacao_a_direita(raiz->direita);
        return rotacao_a_esquerda(raiz);
    }

    return raiz;
}
/**************************************************************************************MAIN*/
int main() {
No *raiz = NULL;

   /*Para inserir um nó*/
    /* raiz = inserir_no(raiz, valor);*/

    raiz = inserir_no(raiz, 30);
    raiz = inserir_no(raiz, 20);
    raiz = inserir_no(raiz, 11);
    raiz = inserir_no(raiz, 1000);
    raiz = inserir_no(raiz, 12);
    raiz = inserir_no(raiz, 14);

    /*Para remover um nó*/
    /*remover_no(raiz, valor);*/



    /*Para printar a árvore em ordem*/
    /*impressao_em_ordem(raiz);*/

    /*Para printar os detalhes e ... */
    imprimir_detalhes(raiz);

    /*Para printar a árvore*/
    imprimir_arvore(raiz, 0);

    return 0;
}