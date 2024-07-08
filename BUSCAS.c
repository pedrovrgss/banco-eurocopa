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
    printf("--------------------------------\n\n");
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

void BUSCA_4(char *raiz, int t){
    const char *narq = "tabs_aux/selecoes.dat";
    FILE *f = fopen(narq, "rb");
    Jogador *jogador = aloca_jogador();
    int contador = 0;
    fread(jogador, sizeof(Jogador), 1, f);
    char selec_atual[20];
    strcpy(selec_atual, jogador -> selecao);
    fseek(f, 0, SEEK_SET);

    while(!feof(f)){
        fread(jogador, sizeof(Jogador), 1, f);
        if (strcmp(selec_atual, jogador -> selecao) == 0) contador++;
        else{
            printf("\n%s - %d jogadores", selec_atual, contador);
            strcpy(selec_atual, jogador -> selecao);
            contador = 1;
        }
    }
    printf("\n%s - %d jogadores\n\n", selec_atual, contador);
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

// ======================================================================================================

void BUSCA_9(char *raiz, int t) {
    const char *narq = "tabs_aux/selecoes.dat";
    FILE *f = fopen(narq, "rb");

    if (f == NULL) {
        printf("Erro ao abrir o arquivo %s\n", narq);
        return;
    }

    Jogador *aux = aloca_jogador();
    if (aux == NULL) {
        printf("Erro ao alocar memória para o jogador.\n");
        fclose(f);
        return;
    }

    char selecao_atual[20], maior_selecao[20];
    int contador = 0, maior_contador = INT_MIN;

    while (fread(aux, sizeof(Jogador), 1, f) == 1) {
        if (contador == 0 || strcmp(aux->selecao, selecao_atual) != 0) {
            if (contador > maior_contador) {
                maior_contador = contador;
                strcpy(maior_selecao, selecao_atual);
            }
            contador = 0;
            strcpy(selecao_atual, aux->selecao);
        }
        contador++;
    }

    // Verifica se a última seleção lida tem o maior contador
    if (contador > maior_contador) {
        maior_contador = contador;
        strcpy(maior_selecao, selecao_atual);
    }

    printf("Selecao com mais jogadores atuando fora do país de origem (%d jogadores): %s\n\n", maior_contador, maior_selecao);

    fclose(f);
    free(aux);
}
// ======================================================================================================

void BUSCA_10(char *raiz, int t) {
    const char *narq = "tabs_aux/selecoes.dat";
    FILE *f = fopen(narq, "rb");

    if (f == NULL) {
        printf("Erro ao abrir o arquivo %s\n", narq);
        return;
    }

    Jogador *aux = aloca_jogador();
    if (aux == NULL) {
        printf("Erro ao alocar memória para o jogador.\n");
        fclose(f);
        return;
    }

    char selecao_atual[20] = "";
    char menor_selecao[20] = "";
    int contador = 0, menor_contador = INT_MAX;
    int primeira_leitura = 1;

    while (fread(aux, sizeof(Jogador), 1, f) == 1) {
        if (primeira_leitura || strcmp(aux->selecao, selecao_atual) != 0) {
            if (!primeira_leitura && contador < menor_contador && contador > 0) {
                menor_contador = contador;
                strcpy(menor_selecao, selecao_atual);
            }
            contador = 0;
            strcpy(selecao_atual, aux->selecao);
            primeira_leitura = 0;
        }
        contador++;
    }

    // Verifica se a última seleção lida tem o menor contador
    if (contador < menor_contador && contador > 0) {
        menor_contador = contador;
        strcpy(menor_selecao, selecao_atual);
    }

    printf("Selecao com menos jogadores atuando fora do país de origem (%d jogadores): %s\n\n", menor_contador, menor_selecao);

    fclose(f);
    free(aux);
}

// ======================================================================================================

void BUSCA_11(char *raiz, int t, int id){
    Jogador *jogador = TARVBS_busca_jogador(raiz, id, t);
    if (!jogador) printf("\n\nNão existe nenhum jogador com esse id.\n\n");
    else{
        print_jogador(jogador);
    }
    return;
}

// ======================================================================================================

void BUSCA_12(char *raiz, int t, int id){
    Jogador *jogador = TARVBS_busca_jogador(raiz, id, t);
    if (!jogador) {
        printf("\n\nNão existe nenhum jogador com esse id.\n\n");
        return;
    }
    free(jogador);

    int i = 0;
    TARVBS *aux = TARVBS_nova(t);
    aux = ler_arquivo(TARVBS_busca(raiz, id, t), t);
    for (i; i < aux -> nchaves; i++) if (aux -> chave[i].id == id) break;


    int opcao;
    printf("\n\nQual dado você deseja alterar?\n\n");
    printf("1. Número da camisa\n");
    printf("2. Posição (exceto GK)\n");
    printf("3. Número de partidas\n");
    printf("4. Número de gols\n");
    printf("5. Capitão\n");
    printf("6. País do time atual\n");
    printf("7. Nome do time atual\n\n");
    printf("Digite a opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
                printf("Digite o novo número da camisa: ");
                scanf("%d", &jogador->camisa);
                aux->chave[i].camisa = jogador->camisa;
                sobrescrever(aux->narq, aux, t);
                printf("Número da camisa alterado com sucesso!\n");
                print_jogador(TARVBS_busca_jogador(raiz, id, t));
                break;
            case 2:
                if (strcmp(jogador->pos, "GK") == 0) {
                    printf("\nNão é possivel alterar a posição desse jogador\n\n");
                    break;
                }
                printf("Digite a nova posição (exceto GK): ");
                scanf("%s", jogador->pos);
                if (strcmp(jogador->pos, "GK") == 0) {
                    printf("\nNão é possivel alterar a posição desse jogador para GK\n\n");
                    break;
                }
                if (strcmp(jogador->pos, "DF") != 0 && strcmp(jogador->pos, "MF") != 0 && strcmp(jogador->pos, "FW") != 0) {
                    printf("\nNão existe essa posição\n\n");
                    break;
                }
                strcpy(aux->chave[i].pos, jogador->pos);
                sobrescrever(aux->narq, aux, t);
                printf("Posição alterada com sucesso!\n");
                print_jogador(TARVBS_busca_jogador(raiz, id, t));
                break;
            case 3:
                printf("Digite o novo número de partidas: ");
                scanf("%d", &jogador->jogos);
                aux->chave[i].jogos = jogador->jogos;
                sobrescrever(aux->narq, aux, t);
                printf("Número de partidas alterado com sucesso!\n");
                print_jogador(TARVBS_busca_jogador(raiz, id, t));
                break;
            case 4:
                printf("Digite o novo número de gols: ");
                scanf("%d", &jogador->gols);
                aux->chave[i].gols = jogador->gols;
                sobrescrever(aux->narq, aux, t);
                printf("Número de gols alterado com sucesso!\n");
                print_jogador(TARVBS_busca_jogador(raiz, id, t));
                break;
            case 5:
                printf("É capitão? (1 para sim, 0 para não): ");
                int capitao;
                scanf("%d", &capitao);
                if (capitao != 1 && capitao != 0) {
                    printf("\nEntrada inválida\n\n");
                    break;
                }
                jogador->capitao = (boolean)capitao;
                aux->chave[i].capitao = jogador->capitao;
                sobrescrever(aux->narq, aux, t);
                printf("Status de capitão alterado com sucesso!\n");
                print_jogador(TARVBS_busca_jogador(raiz, id, t));
                break;
            case 6:
                printf("Digite o novo país do time atual: ");
                scanf("%s", jogador->pais);
                strcpy(aux->chave[i].pais, jogador->pais);
                sobrescrever(aux->narq, aux, t);
                printf("País do time atual alterado com sucesso!\n");
                print_jogador(TARVBS_busca_jogador(raiz, id, t));
                break;
            case 7:
                printf("Digite o novo nome do time atual: ");
                scanf("%s", jogador->time);
                strcpy(aux->chave[i].time, jogador->time);
                sobrescrever(aux->narq, aux, t);
                printf("Nome do time atual alterado com sucesso!\n");
                print_jogador(TARVBS_busca_jogador(raiz, id, t));
                break;
        default:
            printf("Opção inválida!\n");
            break;
    }
}

// ======================================================================================================
void busca13_aux(char *raiz, int t, char *selecao){
    const char* narq = "tabs_aux/selecoes.dat";
    FILE *f = fopen(narq, "rb");
    Jogador *atual = aloca_jogador();

    printf("\nJogadores de %s:\n\n", selecao);
    while (!feof(f)){
        fread(atual, sizeof(Jogador), 1, f);
        if (strcmp(atual -> selecao, selecao) == 0) printf("%s\n", atual -> nome);
    }
    fclose(f);
    free(atual);
    return;
}

void BUSCA_13(char *raiz, int t, int opt){
    switch (opt){
        case 1: busca13_aux(raiz, t, "Germany");
        break;

        case 2: busca13_aux(raiz, t, "Scotland");
        break;

        case 3: busca13_aux(raiz, t, "Croatia");
        break;

        case 4: busca13_aux(raiz, t, "Albania");
        break;

        case 5: busca13_aux(raiz, t, "Slovenia");
        break;

        case 6: busca13_aux(raiz, t, "Denmark");
        break;

        case 7: busca13_aux(raiz, t, "Netherlands");
        break;

        case 8: busca13_aux(raiz, t, "France");
        break;

        case 9: busca13_aux(raiz, t, "Ukraine");
        break;

        case 10: busca13_aux(raiz, t, "Georgia");
        break;

        case 11: busca13_aux(raiz, t, "Portugal");
        break;

        default: printf("Opção inválida");
    }
    return;
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

