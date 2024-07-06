#include "TARVBS.h"

char * id() {
    static int counter = 0;
    counter++;
    
    char *id_str = malloc(20);
    if (id_str == NULL) {
        return NULL;
    }
    sprintf(id_str, "%d", counter);
    return id_str;
}

char *raiz(char *nova){
    static char raiz[24] = "arqvs/arquivo0.bin";

    if (strlen(nova) < 10) return raiz;
    else {
        strncpy(raiz, nova, 24);
        return raiz;
    }
}

void sobrescrever(char *narq, TARVBS *a, int t) {
    FILE *f = fopen(narq, "wb");
    if (!f) {
        perror("Erro ao abrir arquivo para sobrescrever");
        return;
    }

    // Primeiro, escrever a estrutura básica
    fwrite(a, sizeof(TARVBS), 1, f);

    // Em seguida, escrever as chaves
    fwrite(a->chave, sizeof(Jogador), (2 * t - 1), f);

    // Finalmente, escrever os filhos
    for (int i = 0; i < 2 * t; i++) {
        size_t len = a->filho[i] ? strlen(a->filho[i]) + 1 : 0;
        fwrite(&len, sizeof(size_t), 1, f);
        if (len > 0) {
            fwrite(a->filho[i], sizeof(char), len, f);
        }
    }

    fclose(f);
}

Jogador* aloca_jogador() {
    Jogador* novoJogador = (Jogador*)malloc(sizeof(Jogador));
    if (novoJogador == NULL) {
        perror("Falha ao alocar memória para o jogador");
        exit(EXIT_FAILURE); // Encerra o programa em caso de falha
    }
    return novoJogador;
}

TARVBS* ler_arquivo(char *narq, int t) {
    FILE *f = fopen(narq, "rb");
    if (!f) {
        perror("Erro ao abrir arquivo para leitura");
        return NULL;
    }

    TARVBS *a = (TARVBS *)malloc(sizeof(TARVBS));
    if (!a) {
        perror("Erro ao alocar memória para TARVBS");
        fclose(f);
        return NULL;
    }

    fread(a, sizeof(TARVBS), 1, f);

    a->chave = (Jogador *)malloc(sizeof(Jogador) * (2 * t - 1));
    if (!a->chave) {
        perror("Erro ao alocar memória para chaves de TARVBS");
        free(a);
        fclose(f);
        return NULL;
    }
    fread(a->chave, sizeof(Jogador), (2 * t - 1), f);

    a->filho = (char **)malloc(sizeof(char *) * (2 * t));
    if (!a->filho) {
        perror("Erro ao alocar memória para filhos de TARVBS");
        free(a->chave);
        free(a);
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < 2 * t; i++) {
        size_t len;
        fread(&len, sizeof(size_t), 1, f);
        if (len > 0) {
            a->filho[i] = (char *)malloc(len);
            fread(a->filho[i], sizeof(char), len, f);
        } else {
            a->filho[i] = NULL;
        }
    }

    fclose(f);
    return a;
}

TARVBS* TARVBS_nova(int t) {
    TARVBS *nova = (TARVBS *)malloc(sizeof(TARVBS));
    if (!nova) {
        perror("Erro ao alocar memória para TARVBS");
        return NULL;
    }

    nova->chave = (Jogador *)malloc(sizeof(Jogador) * (2 * t - 1));
    if (!nova->chave) {
        perror("Erro ao alocar memória para chaves de TARVBS");
        free(nova);
        return NULL;
    }

    nova->filho = (char **)malloc(sizeof(char *) * (2 * t));
    if (!nova->filho) {
        perror("Erro ao alocar memória para filhos de TARVBS");
        free(nova->chave);
        free(nova);
        return NULL;
    }

    // Alocando espaço e inicializando cada string de filho com ""
    for (int i = 0; i < 2 * t; i++) {
        nova->filho[i] = (char *)malloc(1); // Alocar apenas o espaço para '\0'
        if (!nova->filho[i]) {
            perror("Erro ao alocar memória para filho de TARVBS");
            // Desalocar o que foi alocado até agora
            for (int j = 0; j < i; j++) {
                free(nova->filho[j]);
            }
            free(nova->filho);
            free(nova->chave);
            free(nova);
            return NULL;
        }
        strcpy(nova->filho[i], ""); // Copiar a string vazia para cada filho
    }

    nova->nchaves = 0;
    nova->folha = 1;

    return nova;
}

void printTARVBS(char *narq, int t) {
    TARVBS *a = TARVBS_nova(t);
    a = ler_arquivo(narq, t);

    if (a != NULL) {
        printf("\nArquivo: %s\n", a->narq);
        printf("Numero de chaves: %d\n", a->nchaves);
        printf("Folha: %s\n", a->folha ? "Sim" : "Não");
        printf("Chaves:\n");
        for (int i = 0; i < a->nchaves; i++) {
            printf("\tJogador %d: %s\n", i+1, a->chave[i].nome);
            printf("\n");
        }
        if (!a->folha) {
            int qt_filhos = 0;
            for (int i = 0; a -> filho[i]; i++) {
                printf("\tFilho %d: %s\n\n", i, a -> filho[i]);
                qt_filhos++;
            }
            for (int n = 0; n < qt_filhos; n++) printTARVBS(a -> filho[n], t);
        }
    }
    else return;
}

void TARVBS_imprime(TARVBS *a){
    printf("\nNome do Arquivo: %s\n", a -> narq);
    if (a -> folha) printf("Folha: Sim\n");
    else printf("\nFolha: Nao\n");
    printf("Numero de chaves: %d\n", a->nchaves);
    for (int i = 0; i < a -> nchaves; i++){
        printf("\tChave %d: %s\n", i+1, a -> chave[i].nome);
    }
    if(!a-> folha){
        for (int i = 0; strlen(a -> filho[i]) > 10; i++) printf("Filho %d: %s\n", i+1, a -> filho[i]);
    }
    printf("\n");

}

void freeABS(TARVBS *a, int t) {
    if (a != NULL) {
        if (!a->folha) {
            for (int i = 0; i < 2 * t; i++) {
                if (a->filho[i] != NULL) {
                    free(a->filho[i]);
                }
            }
            free(a->filho);
        }
        free(a->chave);
        free(a);
    }
}

char * TARVBS_cria(int t) { 
    char nome[21] = "arqvs/arquivo";
    strcat(nome, id());
    strcat(nome, ".bin");
    FILE *f = fopen(nome, "wb");
    TARVBS *a = TARVBS_nova(t);
    strcpy(a -> narq, nome);
    sobrescrever(nome, a, t);
    freeABS(a, t);

    char *retorno = (char *)malloc(strlen(nome) + 1);
    if (retorno == NULL) {
        perror("Erro ao alocar memória");
        return NULL;
    }

    // Copiando a string para a área de memória alocada
    strcpy(retorno, nome);

    return retorno;
}

char *TARVBS_busca(char *nome_raiz, int id, int t) {
    FILE *f = fopen(nome_raiz, "rb");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    TARVBS *aux = TARVBS_nova(t);
    if (!aux) {
        fclose(f);
        perror("Erro ao alocar memória para TARVBS");
        return NULL;
    }

    aux = ler_arquivo(nome_raiz, t);

    int i = 0;
    while (i < aux->nchaves && id > aux->chave[i].id) {
        i++;
    }

    if (i < aux->nchaves && id == aux->chave[i].id) {
        char *nome_arquivo = strdup(aux->narq);
        freeABS(aux, t);
        return nome_arquivo;
    }

    if (aux->folha) {
        freeABS(aux, t);
        return NULL;
    }

    char *nome_filho = strdup(aux->filho[i]);
    freeABS(aux, t);
    return TARVBS_busca(nome_filho, id, t);
}

void copiarJogador(Jogador *ptr, Jogador jogador) {
    strcpy(ptr->selecao, jogador.selecao);
    ptr->id = jogador.id;
    ptr->camisa = jogador.camisa;
    strcpy(ptr->pos, jogador.pos);
    strcpy(ptr->nome, jogador.nome);
    ptr->capitao = jogador.capitao;
    ptr->nascimento.dia = jogador.nascimento.dia;
    strcpy(ptr->nascimento.mes, jogador.nascimento.mes);
    ptr->nascimento.ano = jogador.nascimento.ano;
    ptr->idade = jogador.idade;
    ptr->jogos = jogador.jogos;
    ptr->gols = jogador.gols;
    strcpy(ptr->pais, jogador.pais);
    strcpy(ptr->time, jogador.time);
}

Jogador* TARVBS_busca_jogador(char *nome_raiz, int id, int t) {
    char *arquivo = TARVBS_busca(nome_raiz, id, t);
    if (arquivo != NULL) {
        TARVBS *arvore = ler_arquivo(arquivo, t);
        if (arvore != NULL) {
            Jogador *retorno = aloca_jogador();
            if (retorno != NULL) {
                for (int i = 0; i < arvore->nchaves; i++) {
                    if (arvore->chave[i].id == id) {
                        copiarJogador(retorno, arvore->chave[i]);
                        free(arvore->chave);
                        free(arvore->filho);
                        free(arvore);
                        return retorno;
                    }
                }
                free(retorno); // Liberar memória alocada para retorno se não encontrou o jogador
            }
        }
    }
    return NULL;
}

void divisao(char *narq_x, int i, char *narq_y, int t) {
    TARVBS *x = ler_arquivo(narq_x, t);
    if (!x) {
        perror("Erro ao ler arquivo x");
        return;
    }

    TARVBS *y = ler_arquivo(narq_y, t);
    if (!y) {
        perror("Erro ao ler arquivo y");
        freeABS(x, t);
        return;
    }

    char *narq_z = TARVBS_cria(t);
    if (!narq_z) {
        perror("Erro ao criar arquivo z");
        freeABS(x, t);
        freeABS(y, t);
        return;
    }

    TARVBS *z = ler_arquivo(narq_z, t);
    if (!z) {
        perror("Erro ao ler arquivo z");
        freeABS(x, t);
        freeABS(y, t);
        free(narq_z);
        return;
    }

    z->nchaves = t - 1;
    z->folha = y->folha;

    for (int j = 0; j < t - 1; j++) {
        z->chave[j] = y->chave[j + t];
    }

    if (!y->folha) {
        for (int j = 0; j < t; j++) {
            z->filho[j] = strdup(y->filho[j + t]);
            if (!z->filho[j]) {
                perror("Erro ao duplicar string");
                freeABS(x, t);
                freeABS(y, t);
                freeABS(z, t);
                free(narq_z);
                return;
            }
            y->filho[j + t] = NULL;
        }
    }

    y->nchaves = t - 1;

    for (int j = x->nchaves; j >= i; j--) {
        x->filho[j + 1] = x->filho[j];
    }

    x->filho[i] = strdup(narq_z);
    if (!x->filho[i]) {
        perror("Erro ao duplicar string");
        freeABS(x, t);
        freeABS(y, t);
        freeABS(z, t);
        free(narq_z);
        return;
    }

    for (int j = x->nchaves; j >= i; j--) {
        x->chave[j] = x->chave[j - 1];
    }

    x->chave[i - 1] = y->chave[t - 1];
    x->nchaves++;

    sobrescrever(x->narq, x, t);
    sobrescrever(y->narq, y, t);
    sobrescrever(z->narq, z, t);

    freeABS(x, t);
    freeABS(y, t);
    freeABS(z, t);
    free(narq_z);
}

void insere_nao_completo(char *nomearq, Jogador jogador, int t) {
    TARVBS *a = TARVBS_nova(t);
    a = ler_arquivo(nomearq, t);

    int i = a -> nchaves -1;  
    if (a -> folha){
        while (i >= 0 && jogador.id < a -> chave[i].id){
            a -> chave[i+1] = a -> chave[i];
            i--;
        }
        a -> chave[i+1] = jogador;
        a -> nchaves ++;
        sobrescrever(nomearq, a, t);
        freeABS(a, t);
        return;
    }
    while (i >= 0 && jogador.id < a -> chave[i].id) i--;
    i++;
    TARVBS *filho = TARVBS_nova(t);
    filho = ler_arquivo(a -> filho[i], t);

    if (filho -> nchaves == (2*t)-1) {
        divisao(nomearq, (i+1), filho -> narq, t);
        a = ler_arquivo(nomearq, t);
        if (jogador.id > a -> chave[i].id) i++;
    }
    
    freeABS(filho, t);
    insere_nao_completo(a -> filho[i], jogador, t); // e aqui
    freeABS(a, t);
    return;
}

void TARVBS_insere(Jogador jogador, int t) {
    if (TARVBS_busca(raiz(""), jogador.id, t)) return;
    TARVBS *aux = TARVBS_nova(t);

    FILE *f;
    if (!fopen(raiz(""), "rb+")) {
        strcpy(aux -> narq, raiz(""));
    }
    else {
        aux = ler_arquivo(raiz(""), t); 
    }

    if (!aux){
        aux = ler_arquivo(TARVBS_cria(t), t);
        raiz(aux -> narq);
        aux -> chave[0] = jogador;
        aux -> nchaves = 1;
        sobrescrever(raiz(""), aux, t);
        freeABS(aux, t);
        return;
    }
    
    if (aux->nchaves == 0) { // Verificar se a árvore está vazia
        aux->chave[0] = jogador;
        aux->nchaves = 1;
        sobrescrever(aux -> narq, aux, t);
        freeABS(aux, t);
        return;
    }

    //até aqui ta funcionando
    
    if(aux -> nchaves == (2*t)-1){
        TARVBS *s = TARVBS_nova(t);
        s = ler_arquivo(TARVBS_cria(t), t);
        s -> nchaves = 0;
        s -> folha = false;

        
        strcpy(s -> filho[0], aux -> narq);
        sobrescrever(s -> narq, s, t);
        divisao(s->narq, 1, aux -> narq, t);

        if (strcmp(raiz(""), aux -> narq) == 0) {
            raiz(s -> narq);
        }
        
        insere_nao_completo(s -> narq, jogador, t);
        return;
    }

    insere_nao_completo(raiz(""), jogador, t);
    return;
}

/*
insere:
    1. se tiver na arvore retorna a raiz CHECK
    2. se não existir arvore cria um nó folha CHECK
    3. se tiver o maximo de chaves, cria um nó secundário que vira pai do nó com maximo de chaves CHECK
    4. chama a função divisao para dividir o nó com maximo de chaves CHECK
    5. chama a insere não completo CHECK


divisão
    1. cria um nó z que vai ficar na mesma altura do nó y CHECK
    2. divide y em 2 partes, uma parte continua em y e outra fica em z (parte maior em z) CHECK
    3. se y tiver filhos, passa parte dos filhos para z CHECK
    4. ajusta os filhos do que subiu CHECK
    5. ajusta as chaves do que subiu CHECK


insere nao completo
    1. se x for folha vai percorrendo até achar o lugar certo pra inserir, depois ajusta as outras chaves CHECK
    2. acha o filho que tem que descer para inserir, se o filho tiver o maximo de chaves, divide e sobe a chave do meio para a atual
    3. chama recursivo para o nó filho que tem que descer

*/


void TARVBS_retira(Jogador jogador, int t);





































