#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "questao1.h"
// Para um esporte escolhido, mostre o ranking dos países com mais medalhas nesse esporte.
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
            campos[k][j++] = linha[i];
        }
        i++;
    }
    /* O final do último campo também recebe \0.*/
    campos[k][j] = '\0';
    /* Enfim, será retornado o número de campos encontrados. */
    return k + 1;
}
Pais* criarPaises(int* quantidade) {
    Pais* paises = (Pais*) malloc(sizeof(Pais) * 230);
    int indice = 0;
    FILE* noc = fopen("noc_regions.csv", "r");
    int camposMax = 3;
    char campos[camposMax][64];
    char linha[128];
    fgets(linha, sizeof(linha), noc);
    while (fgets(linha, sizeof(linha), noc) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        int NumCampos = separarCampos(linha, campos, camposMax);
        Pais pais;
        strcpy(pais.noc, campos[0]);
        pais.medalhas = 0;
        paises[indice++] = pais;
    }
    *quantidade = indice;
    return paises;
}
void contarMedalhasPorPais(Pais* paises, char* esporte) {
    FILE* results = fopen("results.csv", "r");
    int camposMax = 10;
    char campos[camposMax][64];
    char linha[256];
    fgets(linha, sizeof(linha), results);
    while (fgets(linha, sizeof(linha), results) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        int NumCampos = separarCampos(linha, campos, camposMax);
        char* campoEsporte = campos[8];
        int medalhista = strlen(campos[4]) != 0;
        if (strcmp(campoEsporte, esporte) == 0 && medalhista) {
            char* codigoNoc = campos[7];
            for (int i = 0; i < 230; i++) {
                if (strcmp(paises[i].noc, codigoNoc) == 0) {
                    paises[i].medalhas++;
                }
            }
        }
    }
}
int compararPaises(const void* primeiro, const void* segundo) {
    const Pais *p1 = primeiro;
    const Pais *p2 = segundo;
    if (p1->medalhas != p2->medalhas)
        return p2->medalhas - p1->medalhas;
    return strcmp(p1->noc, p2->noc);
}
void ordenarPorMedalhas(Pais* paises, int quantidade) {
    qsort(paises, quantidade, sizeof(Pais), compararPaises);
}