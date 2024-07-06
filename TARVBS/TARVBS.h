#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int boolean;

#define true 1
#define false 0

typedef struct data{
    int dia;
    char mes[10];
    int ano;
}data;

typedef struct jogador{
    char selecao[15];
    int id;
    int camisa;
    char pos[3];
    char nome[50];
    boolean capitao;
    data nascimento;
    int idade;
    int jogos;
    int gols;
    char pais[20];
    char time[30];
}Jogador;

typedef struct arvbms {
    char narq[24];
    int nchaves;
    Jogador *chave;
    boolean folha;
    char **filho;
}TARVBS;

// Funções

char * TARVBS_cria(int t);
char * TARVBS_busca(char *nome_raiz, int n, int t);
void TARVBS_insere(Jogador jogador, int t);
void TARVBS_retira(Jogador jogador, int t);
