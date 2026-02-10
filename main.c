#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "questao1.h"
#include "questao2.h"
#include "questao3.h"
#include "questao4.h"

int main() {
    int opcao;
    while(true){
        printf("MENU - INFORMACOES SOBRE AS OLIMPIADAS\n");
        printf("Digite um dos numeros a seguir para utilizar o programa:\n");
        printf("1 - Questao 1 - Ranking dos paises por esporte\n");
        printf("2 - Questao 2 - Ranking de 10 paises por numero de atletas\n");
        printf("3 - Questao 3 - Listagem das medalhas conquistadas pelo atleta que chegou ao podio mais vezes\n");
        printf("4 - Questao 4 - Listagem dos paises com mais atletas mulheres\n");
        printf("0 - Sair do programa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        if(opcao == 1){
            questao1exe();
        } else if(opcao == 2){
            questao2exe();
        } else if(opcao == 3){
            questao3exe();
        } else if(opcao == 4){
            questao4exe();
        } else if(opcao == 0){
            printf("Fim do programa. Encerrando...\n");
            break;
        } else{
            printf("Opcao invalida. Tente novamente.\n");
            int limpeza;
            while((limpeza = getchar()) != '\n' && limpeza != EOF);
            getchar();
        }
    }
    return 0;
}