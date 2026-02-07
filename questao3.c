// Ideia: Para um determinado esporte, liste as medalhas conquistadas pelo atleta mais medalhista.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "questao3.h"
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
            campos[k][j++] = linha[i];
        }
        i++;
    }
    /* O final do último campo também recebe \0.*/
    campos[k][j] = '\0';
    /* Enfim, será retornado o número de campos encontrados. */
    return k + 1;
}

/* Função auxiliar que, se o atleta já estiver na array de atletas, retorna sua posição,
mas se não estiver retorna -1. */
int jaEstaNaArray(Atleta* medalhistas, char *ID, int tamanhodaarrray){
    for(int i = 0; i < tamanhodaarrray; i++){
        if(strcmp(medalhistas[i].athlete_id, ID) == 0){
            return i;
        } 
    }
    return -1;
}

/* Função auxiliar para ser usada como argumento de qsort, a qual será a responsável por determinar a
ordenação dos atletas. */
int comparaAtletas(const void *atletaA, const void *atletaB){
    const Atleta *atleta1 = (const Atleta *) atletaA;
    const Atleta *atleta2 = (const Atleta *) atletaB;
    int total1 = atleta1->medalhaBronze + atleta1->medalhaPrata + atleta1->medalhaOuro;
    int total2 = atleta2->medalhaBronze + atleta2->medalhaPrata + atleta2->medalhaOuro;
    return total2 - total1;
}

/* Função que recebe um esporte e devolve uma array contendo o ID e as medalhas dos atletas medalhistas
facilitando o encontro posterior do mais medalhista e da exibição de suas medalhas. */
Atleta* todosAtletasMedalhistas(char *esporteescolhido, int *teste){
    FILE* ARQatletas = fopen("results.csv", "r");
    if(ARQatletas == NULL){
        return NULL;
    }
    /* Como não é possível prever a quantidade de medalhistas por esporte, a array que irá conter os
    dados almejados será feita por alocação dinâmica. Isso, estabelecendo uma quantidade inicial de 100. */
    int capacidade = 100;
    Atleta *Medalhistas = (Atleta*) malloc(capacidade * sizeof(Atleta));
    /*Criando um contador para acompanhar a quantidade de elementos na array para detectar quando for
    necessário usar realloc. */
    int contador = 0;
    
    /* Declarando uma linha de caracteres que irá receber cada linha do arquivo que será lido.*/
    char linha[512];
    /* Descartando o cabeçalho do arquivo csv. */
    fgets(linha, sizeof(linha), ARQatletas);

    /*Através de um laço while, cada linha do arquivo ARQatletas irá sobrescrever a anterior, isso,
    até chegar ao final do arquivo, reduzindo, assim, a quantidade de espaço de memória usado.*/
    while(fgets(linha, sizeof(linha), ARQatletas) != NULL){
        /* Realizando o tratamento da nova linha lida, de modo a substituir o '\n' em seu final por um '\0'.*/
        linha[strcspn(linha, "\n")] = '\0';
        /* Criando uma array que irá receber os campos da linha lida, isso, sendo que esta possui um máximo
        de 11 campos. */
        char campos[11][64];
        separarCampos(linha, campos, 11);
        /* Destacando os campos relevantes para a conclusão da questão.*/
        char *Medal = campos[4];
        char *Nome = campos[5];
        char *ID = campos[6];
        char *Esporte = campos[8];

        /*No arquivo results, um atleta pode ter Medal "", "Bronze", "Silver"," Gold". Ou seja, como a busca
        é pelos atletas medalhista de um esporte específico, só seria necessário armazenar as informações dos atletas
        que atuam tal esporte e forem medalhistas, isto é, seu Medal != "". */
        if(Medal[0] != '\0' && strcmp(esporteescolhido, Esporte) == 0){
            /* testando se o atleta retratado na linha já está na array.*/
            int indice = jaEstaNaArray(Medalhistas, ID, contador);
            /* Se estiver, o seu campo Medal é analisado e a variável compatível com o campo é incrementada.*/
            if(indice != -1){
                if(strcmp(Medal, "Bronze") == 0){
                    Medalhistas[indice].medalhaBronze++;
                } else if (strcmp(Medal, "Silver") == 0){
                    Medalhistas[indice].medalhaPrata++;
                } else if(strcmp(Medal, "Gold") == 0){
                    Medalhistas[indice].medalhaOuro++;
                }
            /* Se não estiver, esse novo atleta medalhista será acrescentado na array. */
            } else {
                /*Se a array estiver em sua capacidade máxima, o realloc irá aumentar a capacidade da array em
                25. */
                if (contador == capacidade) {
                    capacidade += 25;
                    /*Array temporária que receberá o novo tamanho. */
                    Atleta *tmp = realloc(Medalhistas, capacidade * sizeof(Atleta));
                    /*Caso falhe, a memória é liberada, o arquivo fechado e a função retornará NULL. */
                    if (!tmp) {
                        free(Medalhistas);
                        fclose(ARQatletas);
                        return NULL;
                    }
                    /* Agora Medalhistas referencia uma array de maior capacidade. */
                    Medalhistas = tmp;
                }
                /*Inicializando os structs com 0 para os inteiros e "" para as strings. */
                memset(&Medalhistas[contador], 0, sizeof(Atleta));
                /* Inserindo o ID do novo atleta medalhista.*/
                strcpy(Medalhistas[contador].athlete_id, ID);
                /* Inserindo o nome do novo atleta medalhista.*/
                strcpy(Medalhistas[contador].nome, Nome);
                /*Inicializando a medalha que configurou ele como medalhista. */
                if(strcmp(Medal, "Bronze") == 0){
                    Medalhistas[contador].medalhaBronze = 1;
                } else if (strcmp(Medal, "Silver") == 0){
                    Medalhistas[contador].medalhaPrata = 1;
                } else if(strcmp(Medal, "Gold") == 0){
                    Medalhistas[contador].medalhaOuro = 1;
                }
                contador++;
            }
        }
    }
    /* Indicando que "teste" referencia um mesmo valor que "contador", para que, no caso do usuário digitar um esporte
    inexistente, seu valor seja 0..*/
    *teste = contador;
    /* Ordenando os atletas utilizando como parâmetro de comparação a função comparaAtletas anteriormente
    criada, reposicionando os atletas em ordem descrescente, daquele com mais medalhas até aquele com menos.
    Isso, garantindo que o mais medalhista sempre seja o colocado em indice 0. */
    qsort(Medalhistas, contador, sizeof(Atleta), comparaAtletas);
    /* Fechando o arquivo e retornando uma array com todos os medalhistas do esporte selecionado. */
    fclose(ARQatletas);
    return Medalhistas;
}
