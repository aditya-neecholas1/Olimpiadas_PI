#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "questao1.h"
#include "questao2.h"

int main() {
    int opcao;
    while(true){
        printf("MENU - INFORMAÇÕES SOBRE AS OLIMPÍADAS\n");
        printf("Digite um dos numeros a seguir para utilizar o programa:\n");
        printf("1 - Questao 1 - Ranking dos países por esporte\n");
        printf("2 - Questao 2 - Ranking de 10 países por número de atletas\n");
        printf("3 - Questao 3 - ?\n");
        printf("4 - Questao 4 - ?\n");
        printf("0 - Sair do programa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        if(opcao == 1){
            questao1exe();
        } else if(opcao == 2){
            questao2exe();
        } else if(opcao == 3){
            printf("Questao 3 ainda nao foi implementada.\n");
            printf("Aperte Enter para voltar ao menu.");
            limparBuffer();
            getchar();
        } else if(opcao == 4){
            printf("Questao 4 ainda nao foi implementada.\n");
            printf("Aperte Enter para voltar ao menu.");
            limparBuffer();
            getchar();
        } else if(opcao == 0){
            printf("Fim do programa. Encerrando...\n");
            break;
        } else{
            printf("Opcao invalida. Tente novamente.\n");
            limparBuffer();
            getchar();
        }

    }
    return 0;
}

void limparBuffer(){
    int limpeza;
    while((limpeza = getchar()) != '\n' && limpeza != EOF);
}

void questao1exe(){
    // 1º questão: Para um esporte escolhido, mostre o ranking dos países com mais medalhas nesse esporte.
    limparBuffer();
    printf("Digite o nome do esporte que deseja consultar\n> ");
    char esporte[32];
    fgets(esporte, sizeof(esporte), stdin);
    esporte[strcspn(esporte, "\n")] = '\0';
    int quantidade;
    Pais* paises = criarPaises(&quantidade);
    contarMedalhasPorPais(paises, quantidade, esporte);
    ordenarPorMedalhas(paises, quantidade);
    if (paises[0].medalhas == 0) {
        printf("Esse esporte não existe, parça.\n");
        return;
    }
    printf("RANKING DOS PAISES QUE MAIS RECEBEM MEDALHAS EM: %s\n", esporte);
    for (int i = 1; i <= 10; i++) {
        printf("%dº - %s: %d medalhas\n", i, (paises + i)->nome, (paises + i)->medalhas);
    }
    printf("Pressione Enter para voltar.");
    getchar();
}

void questao2exe(){
    /*2ª questão: Considere 10 países a sua escolha. 
    Crie uma lista de países ordenada pelo número total de atletas
    cadastrados, do maior para o menor.*/
    limparBuffer();
    int quantidade;
    PaisAtletas* paisesEscolhidos = criarPaises(&quantidade);
    contarAtletasPorPais(paisesEscolhidos, 10);
    printf("RANKING DOS 10 PAISES COM MAIS ATLETAS:\n");
    for (int i = 0; i < 10; i++) {
        printf("%dº - %s: %d atletas cadastrados\n", i + 1, (paisesEscolhidos + i)->noc, (paisesEscolhidos + i)->atletas);
    }
    printf("Pressione Enter para voltar.");
    getchar();
}