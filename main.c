#include "BUSCAS.c"

void ler_jogadores(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Jogador jogador;

    // Lê cada jogador até o final do arquivo
    while (fread(&jogador, sizeof(Jogador), 1, arquivo) == 1) {
        printf("Nome: %s\n", jogador.nome);
        printf("Idade: %d\n", jogador.idade);
        printf("ID: %d\n", jogador.id);
        printf("Seleção: %s\n", jogador.selecao);
        printf("Posição: %s\n", jogador.pos);
        printf("Camisa: %d\n", jogador.camisa);
        printf("Capitão: %s\n", jogador.capitao ? "Sim" : "Não");
        printf("Data de Nascimento: %d/%s/%d\n", jogador.nascimento.dia, jogador.nascimento.mes, jogador.nascimento.ano);
        printf("Jogos: %d\n", jogador.jogos);
        printf("Gols: %d\n", jogador.gols);
        printf("País: %s\n", jogador.pais);
        printf("Time: %s\n", jogador.time);
        printf("\n");
    }

    fclose(arquivo);
}

int conta_jogadores(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1; // Retorna -1 em caso de erro ao abrir o arquivo
    }

    Jogador jogador;
    int contador = 0;

    while (fread(&jogador, sizeof(Jogador), 1, arquivo) == 1) {
        contador++;
    }

    fclose(arquivo);
    return contador;
}

void delete_criar_pasta(const char *nome_pasta) {
    // Comando para deletar a pasta e seus conteúdos recursivamente
    char comando_delete[100];
    snprintf(comando_delete, sizeof(comando_delete), "rm -rf %s", nome_pasta);
    system(comando_delete);

    // Comando para criar a pasta novamente
    char comando_criar[50];
    snprintf(comando_criar, sizeof(comando_criar), "mkdir %s", nome_pasta);
    system(comando_criar);
}

int opcao() {
    int escolha;

    printf("Selecione uma opcao:\n");
    printf("(1) Retorno do jogador mais novo e mais velho da competicao, ilustrando essas mesmas informacoes por selecoes e por posicoes;\n");
    printf("(2) Os jogadores que mais e menos atuaram em suas equipes;\n");
    printf("(3) Os jogadores que mais e menos atuaram no total;\n");
    printf("(4) A menor e a maior selecao, isto é, com menos ou mais convocados;\n");
    printf("(5) Busca de todos os jogadores que atuam fora do seu país de origem;\n");
    printf("(6) Busca de todos os jogadores que atuam no seu país de origem;\n");
    printf("(7) Busca de todos os jogadores que nasceram no mesmo ano;\n");
    printf("(8) Busca de todos os jogadores que nasceram no mesmo mês;\n");
    printf("(9) Busca da(s) selecao(ções) com mais jogadores que atuam fora do seu país de origem;\n");
    printf("(10) Busca da(s) selecao(ções) com mais jogadores que atuam no seu país de origem;\n");
    printf("(11) Busca das informações subordinadas, dadas a chave primária (identificador único do jogador);\n");
    printf("(12) Alteração SOMENTE de algumas informações, por exemplo, número da camisa que ele usa em sua respectiva seleção, a posição que ele joga (EXCETO GK), o número de partidas pela seleção, o número de gols, o jogador passar a (ou deixar de) ser capitão, o país do seu time atual e o nome do time atual;\n");
    printf("(13) Busca de todos os jogadores de uma selecao;\n");
    printf("(14) Busca e remocao de todos os capitoes;\n");
    printf("(15) Remocao de jogadores a partir de uma determinada idade;\n");
    printf("(16) Retirada de todos os jogadores de uma selecao que atuam num determinado pais;\n");
    printf("(17) Retirada de todos os os jogadores de uma selecao que nao atuam no país de origem;\n");
    printf("(18) Retirada de todos os os jogadores de uma selecao atuam fora do pais de origem;\n");
    printf("(19) Retirada de todos os os jogadores de uma selecao; e\n");
    printf("(20) Remocao de jogadores, dado um conjunto contendo suas chaves primarias.\n");
    printf("\n(21) SAIR");

    printf("\n\nDigite a opcao desejada: ");
    while (scanf("%d", &escolha) != 1 || escolha < 1 || escolha > 21) {
        printf("\n\nOpcao invalida. Tente novamente: ");
        while (getchar() != '\n');
    }

    return escolha;
}

int main(){
    delete_criar_pasta("arqvs");
    delete_criar_pasta("tabs_aux");
    int id_selec;
    int escolha;
    int t;
    int selecao7;

    printf("Digite o grau minimo (Se o valor digitado for menor que 2, t sera considerado igual a 2): ");
    scanf("%d", &t);

    CriaArvore("EURO.txt", t);
    printf("\n\nARVORE CRIADA\n\n");

    CRIA_TABELA_SELECOES(t);
    printf("\nTABELA SELEÇÕES CRIADA\n");
    printf("%d jogadores no total.\n\n\n", conta_jogadores("tabs_aux/selecoes.dat"));

    boolean loop = true;
    while (loop) {
        escolha = opcao();
        printf("\n\n");

        switch (escolha){
            case 1:
                BUSCA_1(raiz(""), t);
                break;
            case 2:
                BUSCA_2(raiz(""), t);
                break;
            
            case 5:
                BUSCA_5(raiz(""), t);
                break;

            case 6:
                BUSCA_6(raiz(""), t);
                break;
            
            case 11: 
                printf("\nDigite o id do jogador que você quer os dados: ");
                scanf("%d", &id_selec);
                BUSCA_11(raiz(""), t, id_selec);
                break;
            
            case 12:
                printf("\nDigite o id do jogador que você quer alterar os dados: ");
                scanf("%d", &id_selec);
                BUSCA_12(raiz(""), t, id_selec);
                break;

            case 13:
                printf("1. Germany\n");
                printf("2. Scotland\n");
                printf("3. Croatia\n");
                printf("4. Albania\n");
                printf("5. Slovenia\n");
                printf("6. Denmark\n");
                printf("7. Netherlands\n");
                printf("8. France\n");
                printf("9. Ukraine\n");
                printf("10. Georgia\n");
                printf("11. Portugal\n\n");
                printf("Escolha um numero: ");

                scanf("%d", &selecao7);

                BUSCA_13(raiz(""), t, selecao7);

            case 21:
                loop = false;
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    }

    delete_criar_pasta("arqvs");
    delete_criar_pasta("tabs_aux");

    return 0;
}


// criar a arvore b+ dos arqvs binarios

// criar 11 tabelas hash ordenadas (em ms) cada uma por um parametro diferente e com a informação do endereço de cada jogador na árvore

// fazer as funções de busca de preferencia em um arquivo separado apenas para essas funções de busca