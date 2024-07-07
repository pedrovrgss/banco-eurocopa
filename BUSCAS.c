#include "CRIA_TABELAS.c"
#include "TLSE/TLSE.c"
#include <limits.h>

boolean existe_arquivo(const char* nome){
    FILE *f = fopen(nome, "rb");
    if (!f) return false;
    else return true;
}

// ======================================================================================================

void busca1_aux(char *raiz, int t, TLSE **maior, TLSE **menor, int *maior_idade, int *menor_idade) {
    TARVBS *aux = TARVBS_nova(t);
    aux = ler_arquivo(raiz, t);
    if (!aux) {
        perror("Arvore vazia");
        return;
    }

    if (aux->folha == false) {
        for (int j = 0; j < t*2; j++) {  // Supondo que t*2 é o número máximo de filhos
            if (aux->filho[j] != NULL && strlen(aux->filho[j]) > 10) {
                busca1_aux(aux->filho[j], t, maior, menor, maior_idade, menor_idade);
            }
        }
    }

    for (int i = 0; i < aux->nchaves; i++) {
        if (aux->chave[i].idade > *maior_idade) {
            *maior_idade = aux->chave[i].idade;
            TLSE_lib_rec(*maior);
            *maior = NULL;
            *maior = TLSE_insere(*maior, aux->chave[i].id);
        }
        else if (aux->chave[i].idade == *maior_idade) {
            *maior = TLSE_insere(*maior, aux->chave[i].id);
        }
        if (aux->chave[i].idade < *menor_idade) {
            *menor_idade = aux->chave[i].idade;
            TLSE_lib_rec(*menor);
            *menor = NULL;
            *menor = TLSE_insere(*menor, aux->chave[i].id);
        }
        else if (aux->chave[i].idade == *menor_idade) {
            *menor = TLSE_insere(*menor, aux->chave[i].id);
        }
    }

    freeABS(aux, t);
}


void BUSCA_1(char *raiz, int t){
    TLSE *mais_velhos = TLSE_inicializa(), *mais_novos = TLSE_inicializa();
    int maior_idade = INT_MIN, menor_idade = INT_MAX;

    busca1_aux(raiz, t, &mais_velhos, &mais_novos, &maior_idade, &menor_idade);

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

    TLSE_lib_rec(mais_novos);
    TLSE_lib_rec(mais_velhos);
}

// ======================================================================================================
void print_jogador(const Jogador *jogador) {
    printf("Selecao: %s\n", jogador->selecao);
    printf("ID: %d\n", jogador->id);
    printf("Camisa: %d\n", jogador->camisa);
    printf("Posicao: %s\n", jogador->pos);
    printf("Nome: %s\n", jogador->nome);
    printf("Capitao: %s\n", jogador->capitao ? "Sim" : "Nao");
    printf("Data de nascimento: %d %s %d\n", jogador->nascimento.dia, jogador->nascimento.mes, jogador->nascimento.ano);
    printf("Idade: %d\n", jogador->idade);
    printf("Jogos: %d\n", jogador->jogos);
    printf("Gols: %d\n", jogador->gols);
    printf("Pais: %s\n", jogador->pais);
    printf("Time: %s\n", jogador->time);
    printf("--------------------------------\n");
}

void BUSCA_2(char *raiz, int t) {
    const char *narq = "tabs_aux/selecoes.dat";
    FILE *f = fopen(narq, "rb");
    char selecao_atual[15];
    Jogador *aux = aloca_jogador(), *aux2 = aloca_jogador();
    fread(aux, sizeof(Jogador), 1, f);
    strcpy(selecao_atual, aux -> selecao);
    int maisjogos = INT_MIN, menosjogos = INT_MAX;

    TLSE *ls_mais = NULL, *ls_menos = NULL;

    while(!feof(f)) {
        while (strcmp(selecao_atual, aux -> selecao) == 0){
            if (aux->jogos > maisjogos) {
                maisjogos = aux->jogos;
                TLSE_lib_rec(ls_mais);
                ls_mais = NULL;
                ls_mais = TLSE_insere(ls_mais, aux->id);
            }
            else if (aux->jogos == maisjogos) {
                ls_mais = TLSE_insere(ls_mais, aux->id);
            }
            if (aux->jogos < menosjogos) {
                menosjogos = aux->jogos;
                TLSE_lib_rec(ls_menos);
                ls_menos = NULL;
                ls_menos = TLSE_insere(ls_menos, aux->id);
            }
            else if (aux->jogos == menosjogos) {
                ls_menos = TLSE_insere(ls_menos, aux->id);
            }
            fread(aux, sizeof(Jogador), 1, f);
        }
        
        printf("SELEÇÃO: %s\n\n", selecao_atual);
        printf("\tMAIS JOGOS:\n");
        while(ls_mais){
            aux2 = TARVBS_busca_jogador(raiz, ls_mais -> info, t);
            printf("\t\t%s, %d jogos.\n", aux2 -> nome, aux2 -> jogos);
            ls_mais = ls_mais -> prox;
        }
        printf("\tMENOS JOGOS:\n");
        while(ls_menos){
            aux2 = TARVBS_busca_jogador(raiz, ls_menos -> info, t);
            printf("\t\t%s, %d jogos.\n", aux2 -> nome, aux2 -> jogos);
            ls_menos = ls_menos -> prox;
        }
        printf("\n");

        strcpy(selecao_atual, aux -> selecao);
        TLSE_lib_rec(ls_mais);
        TLSE_lib_rec(ls_menos);
        maisjogos = INT_MIN;
        menosjogos = INT_MAX;
    }
    free(aux);
    free(aux2);
    fclose(f);
    
    return;
}

// ======================================================================================================

boolean compara_selecao_pais(const char selecao[20], const char pais[20]){
    for (int i = 0; i < 20; i++){
        if (pais[i] != selecao[i]){
            return false;
        }
    }
    return true;
}

void busca5_aux(char *raiz, int t, TLSE **l) {
    TARVBS *a = ler_arquivo(raiz, t);

    if (!a){
        perror("Erro ao abrir a arvore");
        return;
    }

    if (a->folha == false) {
        for (int j = 0; existe_arquivo(a -> filho[j]); j++) {

            busca5_aux(a -> filho[j], t, l);
        }
    }


    for (int i = 0; i < a->nchaves; i++) {
        if (!compara_selecao_pais(a -> chave[i].selecao, a -> chave[i].pais)){
            *l = TLSE_insere(*l, a ->chave[i].id);
        }
    }

    return;
}

void BUSCA_5(char *raiz, int t) {
    TLSE *lista = TLSE_inicializa();
    Jogador *atual;

    busca5_aux(raiz, t, &lista);

    printf("\n\nLISTA DOS JOGADORES QUE JOGAM FORA DO PAÍS DE ORIGEM: \n\n");
    while (lista){
        atual = TARVBS_busca_jogador(raiz, lista -> info, t);
        printf("%s: \n\tSeleção que joga: %s\n\tPaís que joga: %s\n\n", atual -> nome, atual -> selecao, atual -> pais);
        lista = lista -> prox;
    }
    
}

// ======================================================================================================

void busca6_aux(char *raiz, int t, TLSE **l) {
    TARVBS *a = ler_arquivo(raiz, t);

    if (!a){
        perror("Erro ao abrir a arvore");
        return;
    }

    if (a->folha == false) {
        for (int j = 0; existe_arquivo(a -> filho[j]); j++) {

            busca5_aux(a -> filho[j], t, l);
        }
    }
    for (int i = 0; i < a->nchaves; i++) {
        if (compara_selecao_pais(a -> chave[i].selecao, a -> chave[i].pais)){
            *l = TLSE_insere(*l, a ->chave[i].id);
        }
    }

    return;
}

void BUSCA_6(char *raiz, int t) {
    TLSE *lista = TLSE_inicializa();
    Jogador *atual;

    busca5_aux(raiz, t, &lista);

    printf("\n\nLISTA DOS JOGADORES QUE JOGAM NO PAÍS DE ORIGEM: \n\n");
    while (lista){
        atual = TARVBS_busca_jogador(raiz, lista -> info, t);
        printf("%s: \n\tSeleção: %s\n\n", atual -> nome, atual -> selecao);
        lista = lista -> prox;
    }
    
}



/*
        ESTRUTURA PADRÃO PRA BUSCA NA ÁRVORE

    void busca{n}_aux(char *raiz, int t) {
        TARVBS *a = ler_arquivo(raiz, t);

        if (a->folha == false) {
            for (int j = 0; strlen(a->filho[j]) > 10; j++) {
                busca{n}_aux(a -> filho[j], t);
            }
        }

        for (int i = 0; i < a->nchaves; i++) {
        {aqui entra a lógica da busca}
    }

}
*/

