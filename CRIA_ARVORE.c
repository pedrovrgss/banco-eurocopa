#include <stdio.h>
#include <string.h>
#include "TARVBS/TARVBS.c" 

Jogador criar_jogador(char *linha, const char *selecao) {
    Jogador jogador;
    char nascimento[30];
    int idade;
    char mes[10];
    char nome[50];
    char *pos_abre_parenteses;

    // Inicializar as strings da estrutura jogador para garantir que não haja lixo
    memset(&jogador, 0, sizeof(jogador));

    // Ler os dados formatados do jogador
    sscanf(linha, "%d/%d/%2s/%29[^/]/ %[^(](aged %d)/%d/%d/%19[^/]/%24[^\n]",
           &jogador.id,
           &jogador.camisa,
           jogador.pos,
           nome,
           nascimento,
           &idade,
           &jogador.jogos,
           &jogador.gols,
           jogador.pais,
           jogador.time);

    // Verificar se o jogador é capitão
    pos_abre_parenteses = strchr(nome, '(');
    if (pos_abre_parenteses != NULL) {
        *pos_abre_parenteses = '\0'; // Remove tudo a partir do '('
        jogador.capitao = true;
    } else {
        jogador.capitao = false;
    }

    sscanf(nascimento, "%d %s %d", &jogador.nascimento.dia, mes, &jogador.nascimento.ano);
    strncpy(jogador.nascimento.mes, mes, sizeof(jogador.nascimento.mes) - 1);
    jogador.nascimento.mes[sizeof(jogador.nascimento.mes) - 1] = '\0';

    strncpy(jogador.selecao, selecao, sizeof(jogador.selecao) - 1);
    jogador.selecao[sizeof(jogador.selecao) - 1] = '\0';

    strncpy(jogador.nome, nome, sizeof(jogador.nome) - 1);
    jogador.nome[sizeof(jogador.nome) - 1] = '\0';

    jogador.idade = idade;

    return jogador;
}

void CriaArvore(char *nome_arq, int t) {
    FILE *banco = fopen(nome_arq, "r");
    if (banco == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    char linha[400];
    char paisatual[20] = "";
    Jogador jogador;

    while (fgets(linha, sizeof(linha), banco)) {
        linha[strcspn(linha, "\n")] = '\0';


        if (strchr(linha, '/') == NULL) {
            strncpy(paisatual, linha, sizeof(paisatual) - 1);
            paisatual[sizeof(paisatual) - 1] = '\0';
        } else {
            jogador = criar_jogador(linha, paisatual);
            TARVBS_insere(jogador, t);
            printf("Jogador '%s' adicionado com sucesso!\n", jogador.nome);
        }
    }

    fclose(banco);
}
