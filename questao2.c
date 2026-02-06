#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "questao2.h"

int separarCampos(char *linha, char campos[][64], int camposMax) {
    /*A variável i vai percorrer a linha inteira,
    a variável j vai percorrer os caracteres do campo (por isso é resetada ao
    seu valor inicial de 0 a cada novo campo encontrado.),
    a variável k será usada para percorrer a posição de cada campo na array
    de campos.*/
    int i = 0, j = 0, k = 0;
    /* Para que os campos fossem separados corretamente, mostrou-se necessário
    averiguar se a vírgula faz parte do campo ou se realmente é um separador de campos.
    Desse modo, foi criado um booleano que determina se está ou não dentro de aspas,
    estando, em primeiro momento, fora destas. */
    int dentroDeAspas = 0;
    /* Enquanto a linha não for totalmente percorrida ou quando o número máximo de
    campos desejados for atingido, faça...*/
    while (linha[i] != '\0' && k < camposMax) {
        /* Sempre que uma aspa é encontrada, o estado de dentro de aspas é modificado,
        simbolizando a abertura e o fechamento destas. */
        if (linha[i] == '"') {
            dentroDeAspas = !dentroDeAspas;
        } else if (linha[i] == ',' && !dentroDeAspas){
            /* Se encontrar a vírgula fora das aspas, ocorre a separação, é passado para o
            próximo campo e a variável que percorre os caracteres dos campos é reiniciada. */
            campos[k++][j] = '\0';
            j = 0;
        } else {
            /* Se algo diferente de vírgula ou algo dentro de aspas é percorrido, este caractere
            é copiado como parte do campo. */
            //Acrescentado: verifica se não vai passar do tamanho do campo (63 caracteres + '\0')
            if(j <63) {
            campos[k][j++] = linha[i];
            }
        }
        i++;
    }
    /* O final do último campo também recebe \0.*/
    campos[k][j] = '\0';
    /* Enfim, será retornado o número de campos encontrados. */
    return k + 1;
}

PaisAtletas* criarPaises(int *quantidade){
    // São somente 10 paises a serem escolhidos
    PaisAtletas* paisesEscolhidos = (PaisAtletas*) malloc(sizeof(PaisAtletas) * 10);
    if(!paisesEscolhidos) return NULL;
    printf("Digite o NOC dos paises desejados:\n");
    for(int i = 0; i < 10; i++){
        printf("Pais %d: ", i+1);
        scanf("%s", paisesEscolhidos[i].noc);
        paisesEscolhidos[i].atletas = 0;
    }
    *quantidade = 10;
    return paisesEscolhidos;
}

void contarAtletasPorPais(PaisAtletas* paises, int quantidade){
    FILE* arquivo = fopen("bios.csv", "r");
    if(!arquivo) return;
    char linha[1000];
    char campos[10][64];
    fgets(linha, sizeof(linha), arquivo);
    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        separarCampos(linha, campos, 10);
        char* paisLido = campos[6];
        for(int i = 0; i < quantidade; i++){
            if(strcmp(paises[i].noc, paisLido) == 0){
                paises[i].atletas++;
                break;
            }
        }
    }
    fclose(arquivo);
}

 
