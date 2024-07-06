#include "CRIA_ARVORE.c"

int main(){
    int t;
    printf("Digite o grau minimo (Se o valor digitado for menor que 2, t sera considerado igual a 2): ");
    scanf("%d", &t);

    CriaArvore("EURO.txt", t);
    printf("\n\nARVORE CRIADA");

    return 0;
}


// criar a arvore b+ dos arqvs binarios

// criar 11 tabelas hash ordenadas (em ms) cada uma por um parametro diferente e com a informação do endereço de cada jogador na árvore

// fazer as funções de busca de preferencia em um arquivo separado apenas para essas funções de busca