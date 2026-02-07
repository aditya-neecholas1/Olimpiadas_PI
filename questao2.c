#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "questao2.h"

static int separarCampos(char *linha, char campos[][64], int camposMax) {
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

// Aloca a memória para a lista de países e solicita a digitação dos nomes dos países pelo usuário.
PaisAtletas* criarPaisesAtletas(int *quantidade){
    // Alocação dinâmica para 10 países
    PaisAtletas* paisesEscolhidos = (PaisAtletas*) malloc(sizeof(PaisAtletas) * 10);
    if(!paisesEscolhidos) return NULL; // Checa se a alocação deu certo
    printf("Digite o nome dos paises desejados (em ingles):\n");
    for(int i = 0; i < 10; i++){
        printf("Pais %d: ", i+1); // Orientação para o usuário
        scanf("%s", paisesEscolhidos[i].noc); // Leitura dos nomes
        paisesEscolhidos[i].atletas = 0;
    }
    *quantidade = 10; // Atualiza a variável de controle da quantidade (por padrão é 10)
    return paisesEscolhidos;
}

// Lê o arquivo bios.csv e realiza a contagem de atletas por país escolhidos previamente
void contarAtletasPorPais(PaisAtletas* paises, int quantidade){
    FILE* arquivo = fopen("bios.csv", "r");
    if(!arquivo) return; // Checa se a leitura foi realizada corretamente
    char linha[1000];
    char campos[10][64];
    fgets(linha, sizeof(linha), arquivo); // Descarta o cabeçalho
    // Loop de leitura
    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        separarCampos(linha, campos, 10);
        // O nome do país/NOC está na coluna 6 do arquivo
        char* paisLido = campos[6];
        for(int i = 0; i < quantidade; i++){
            /* Verifica se o nome digitado está contido no nome,
            prevenindo que casos como "Russia Federation" não sejam identificados
            */
            if(strstr(paisLido, paises[i].noc) != NULL){
                paises[i].atletas++;
                break; // Casho encontre, não precisa testar para os outros países
            }
        }
    }
    fclose(arquivo);
}

void ordenarPorAtletas(PaisAtletas* paises, int quantidade){
    /* Utilização do qsort padrão do C para ordenar o vetor, utilizando uma função auxiliar
    compararPaisesAtletas como parâmetro */ 
    qsort(paises, quantidade, sizeof(PaisAtletas), compararPaisesAtletas);
}
int compararPaisesAtletas(const void* primeiro, const void* segundo) {
    // Converte os ponteiros para o tipo 'PaisAtletas'
    const PaisAtletas *p1 = primeiro;
    const PaisAtletas *p2 = segundo;
    // Caso o número de atletas de cada país for diferente, ordena pelo maior número
    if (p1->atletas != p2->atletas)
        return p2->atletas - p1->atletas;
    // Em caso de empate, compara os países pelo nome (ordem lexicográfica)
    return strcmp(p1->noc, p2->noc);
}
 
