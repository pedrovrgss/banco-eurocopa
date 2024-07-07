#include "CRIA_ARVORE.c"

void insere_jogador_ordenado(Jogador novo_jogador, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (arquivo == NULL) {
        // Se o arquivo não existe, cria-se um novo
        arquivo = fopen(nome_arquivo, "wb+");
        if (arquivo == NULL) {
            perror("Erro ao abrir o arquivo");
            return;
        }
    }

    Jogador jogador_atual;
    long posicao_inserir = -1;
    long posicao_atual = ftell(arquivo);

    // Encontra o ponto de inserção ordenada por seleção e ID
    while (fread(&jogador_atual, sizeof(Jogador), 1, arquivo) == 1) {
        int comparacao_pais = strcmp(novo_jogador.selecao, jogador_atual.selecao);
        if (comparacao_pais < 0 || (comparacao_pais == 0 && novo_jogador.id < jogador_atual.id)) {
            posicao_inserir = posicao_atual;
            break;
        }
        posicao_atual = ftell(arquivo);
    }

    if (posicao_inserir == -1) {
        // Se não encontrou posição, insere no final do arquivo
        fseek(arquivo, 0, SEEK_END);
        posicao_inserir = ftell(arquivo);
    }

    // Move os jogadores para abrir espaço para o novo jogador, se necessário
    fseek(arquivo, 0, SEEK_END);
    long tamanho_resto = ftell(arquivo) - posicao_inserir;

    if (tamanho_resto > 0) {
        char *buffer = malloc(tamanho_resto);
        if (buffer == NULL) {
            perror("Erro ao alocar memória");
            fclose(arquivo);
            return;
        }
        fseek(arquivo, posicao_inserir, SEEK_SET);
        fread(buffer, tamanho_resto, 1, arquivo);
        fseek(arquivo, posicao_inserir, SEEK_SET);
        fwrite(&novo_jogador, sizeof(Jogador), 1, arquivo);
        fwrite(buffer, tamanho_resto, 1, arquivo);
        free(buffer);
    } else {
        fseek(arquivo, posicao_inserir, SEEK_SET);
        fwrite(&novo_jogador, sizeof(Jogador), 1, arquivo);
    }

    fclose(arquivo);
}

void cria_arquivo(const char *nome){
    FILE *f = fopen(nome, "wb");
    if (!f) perror("Erro ao criar arquivo");
    fclose(f);        
}

void selecoes_aux(char *raiz, int t) {
    TARVBS *a = TARVBS_nova(t);
    a = ler_arquivo(raiz, t);
    if (!a) {
        perror("Arvore vazia");
        return;
    }
    
    if (!a->folha) {
        for (int j = 0; j <= a->nchaves; j++) {
            selecoes_aux(a->filho[j], t);
        }
    }

    for (int i = 0; i < a->nchaves; i++) {
        insere_jogador_ordenado(a->chave[i], "tabs_aux/selecoes.dat");
    }
    freeABS(a, t);
}


void CRIA_TABELA_SELECOES(int t){
    cria_arquivo("tabs_aux/selecoes.dat");
    selecoes_aux(raiz(""), t);
}