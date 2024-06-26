#include <stdio.h>
#include <string.h>
#include "TARVBM/TARVBM.h"

/* (TARVBMS.h)
#ifndef TARVBMS_H
#define TARVBMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int endereco;
typedef int boolean;

#define true 1
#define false 0

typedef struct jogador {
    int id;
    int numero_camisa;
    char posicao[3];
    char nome[50];
    char data_nascimento[20];
    int partidas_selecao;
    int gols_selecao;
    char pais_time_atual[50];
    char nome_time_atual[50];
} Jogador;

typedef struct arvbm {
    int nchaves, *chave;
    boolean folha;
    endereco *filho, prox;
} TARVBMS;

TARVBMS* TARVBMS_cria(int t);
TARVBMS* TARVBMS_inicializa(void);
TARVBMS* TARVBMS_busca(TARVBMS *a, int mat, FILE *arquivo);
TARVBMS* TARVBMS_insere(TARVBMS *T, int mat, int t, FILE *arquivo);
TARVBMS* TARVBMS_retira(TARVBMS* arv, int k, int t, FILE *arquivo);
void TARVBMS_libera(TARVBMS *a);
void TARVBMS_imprime(TARVBMS *a, FILE *arquivo);
void TARVBMS_imprime_chaves(TARVBMS *a, FILE *arquivo);
void TARVBMS_salva(TARVBMS *no, FILE *arquivo);
TARVBMS* TARVBMS_carrega(endereco endereco_no, FILE *arquivo);
void carregar_dados(const char* nome_arquivo, TARVBMS* arvore, int t, FILE* arquivo);

#endif // TARVBMS_H


-----------------------------------------

(TARVBMS.C) ->Source File
#include "TARVBMS.h"

TARVBMS* TARVBMS_cria(int t) {
    TARVBMS* novo = (TARVBMS*)malloc(sizeof(TARVBMS));
    novo->nchaves = 0;
    novo->chave = (int*)malloc(sizeof(int) * ((t * 2) - 1));
    novo->folha = true;
    novo->filho = (endereco*)malloc(sizeof(endereco) * (t * 2));
    novo->prox = -1;
    for (int i = 0; i < (t * 2); i++) novo->filho[i] = -1;
    return novo;
}

TARVBMS* TARVBMS_inicializa(void) {
    return NULL;
}

void TARVBMS_libera(TARVBMS *a) {
    if (a) {
        free(a->chave);
        free(a->filho);
        free(a);
    }
}

TARVBMS* TARVBMS_busca(TARVBMS *a, int mat, FILE *arquivo) {
    if (!a) return NULL;
    int i = 0;
    while ((i < a->nchaves) && (mat > a->chave[i])) i++;
    if ((i < a->nchaves) && (a->folha) && (mat == a->chave[i])) return a;
    if (a->folha) return NULL;
    if (a->chave[i] == mat) i++;
    TARVBMS* filho = TARVBMS_carrega(a->filho[i], arquivo);
    return TARVBMS_busca(filho, mat, arquivo);
}

void TARVBMS_imprime_chaves(TARVBMS *a, FILE *arquivo) {
    if (!a) return;
    TARVBMS *p = a;
    while (p->filho[0] != -1) p = TARVBMS_carrega(p->filho[0], arquivo);
    while (p) {
        for (int i = 0; i < p->nchaves; i++) printf("%d ", p->chave[i]);
        if (p->prox == -1) break;
        p = TARVBMS_carrega(p->prox, arquivo);
    }
    printf("\n");
}

void TARVBMS_salva(TARVBMS *no, FILE *arquivo) {
    fseek(arquivo, no->nchaves * sizeof(TARVBMS), SEEK_SET);
    fwrite(no, sizeof(TARVBMS), 1, arquivo);
}

TARVBMS* TARVBMS_carrega(endereco endereco_no, FILE *arquivo) {
    fseek(arquivo, endereco_no * sizeof(TARVBMS), SEEK_SET);
    TARVBMS* no = (TARVBMS*)malloc(sizeof(TARVBMS));
    fread(no, sizeof(TARVBMS), 1, arquivo);
    return no;
}
------------------------------------------------
(teste.c)
#include "TARVBMS.h"

int main(int argc, char *argv[]) {
    TARVBMS *arvore = TARVBMS_inicializa();
    int t;
    printf("Digite o grau minimo. Se o valor digitado for menor que 2, t sera considerado igual a 2...\n");
    scanf("%d", &t);
    if (t < 2) t = 2;

    FILE *arquivo = fopen("arvoreBplus.bin", "wb+");

    int num = 0;
    while (num != -1) {
        printf("Digite um numero para adicionar. 0 para imprimir. -9 para remover e -1 para sair\n");
        scanf("%d", &num);
        if (num == -9) {
            int from;
            scanf("%d", &from);
            arvore = TARVBMS_retira(arvore, from, t, arquivo);
            TARVBMS_imprime(arvore, arquivo);
            printf("\n\n");
            TARVBMS_imprime_chaves(arvore, arquivo);
        } else if (num == -1) {
            printf("\n");
            TARVBMS_imprime(arvore, arquivo);
            printf("\n\n");
            TARVBMS_imprime_chaves(arvore, arquivo);
            TARVBMS_libera(arvore);
            fclose(arquivo);
            return 0;
        } else if (!num) {
            printf("\n");
            TARVBMS_imprime(arvore, arquivo);
            printf("\n\n");
            TARVBMS_imprime_chaves(arvore, arquivo);
        } else {
            arvore = TARVBMS_insere(arvore, num, t, arquivo);
        }
        printf("\n\n");
    }

    fclose(arquivo);
    return 0;
}


*/
