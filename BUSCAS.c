#include "CRIA_ARVORE.c"
#include "TLSE/TLSE.c"
#include <limits.h>

void busca_aux(char *raiz, int t, TLSE **maior, TLSE **menor, int *maior_idade, int *menor_idade) {  // maior idade inicial = INT_MIN;    menor idade inicial = INT_MAX;
    TARVBS *aux = TARVBS_nova(t);
    aux = ler_arquivo(raiz, t);
    if (!aux) {
        perror("Arvore vazia");
        return;
    }

    if (aux->folha == false) {
        for (int j = 0; strlen(aux->filho[j]) > 10; j++) {
            busca_aux(aux->filho[j], t, maior, menor, maior_idade, menor_idade);
        }
    }

    for (int i = 0; i < aux->nchaves; i++) {
        if (aux->chave[i].idade > *maior_idade) {
            *maior_idade = aux->chave[i].idade;
            TLSE_libera(*maior);
            *maior = NULL;
        }
        if (aux->chave[i].idade == *maior_idade) {
            *maior = TLSE_insere(*maior, aux->chave[i].id);
        }

        if (aux->chave[i].idade < *menor_idade) {
            *menor_idade = aux->chave[i].idade;
            TLSE_libera(*menor);
            *menor = NULL;
        }
        if (aux->chave[i].idade == *menor_idade) {
            *menor = TLSE_insere(*menor, aux->chave[i].id);
        }
    }

    free(aux);
}

void BUSCA_1(char *raiz, int t){
    TLSE *mais_velhos, *mais_novos;
    int maior_idade = INT_MIN, menor_idade = INT_MAX;

    busca_aux(raiz, t, &mais_velhos, &mais_novos, &maior_idade, &menor_idade);

    Jogador *jogador_atual;
    printf("Mais velho(s): \n");
    while (mais_velhos){
        jogador_atual = TARVBS_busca_jogador(raiz, mais_velhos -> info, t);
        printf("\t%s, %d anos\n", jogador_atual -> nome, jogador_atual -> idade);
        mais_velhos = mais_velhos -> prox;
    }

    printf("Mais novo(s): \n");
    while (mais_novos){
        jogador_atual = TARVBS_busca_jogador(raiz, mais_novos -> info, t);
        printf("\t%s, %d anos\n", jogador_atual -> nome, jogador_atual -> idade);
        mais_novos = mais_novos -> prox;
    }
}