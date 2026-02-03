#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "questao1.h"

int main() {
    // 1º questão: Para um esporte escolhido, mostre o ranking dos países com mais medalhas nesse esporte.
    printf("Digite o nome do esporte que deseja consultar\n> ");
    char esporte[32];
    fgets(esporte, sizeof(esporte), stdin);
    esporte[strcspn(esporte, "\n")] = '\0';
    int quantidade;
    Pais* paises = criarPaises(&quantidade);
    contarMedalhasPorPais(paises, esporte);
    ordenarPorMedalhas(paises, quantidade);
    if (paises[0].medalhas == 0) {
        printf("Esse esporte não existe, parça.\n");
        return 1;
    }
    printf("RANKING DOS PAISES QUE MAIS RECEBEM MEDALHAS EM: %s\n", esporte);
    for (int i = 1; i <= 10; i++) {
        printf("%s recebeu %d medalhas\n", (paises + i)->noc, (paises + i)->medalhas);
    }
    return 0;
}