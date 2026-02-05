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
/* Função responsável por criar um array contendo todos os países presente no arquivo "noc_regions".
As estrutura Pais contém um campo que armazena o código NOC do país e outro que armazena a quantidade de medalhas acumuladas. 
A função retorna uma referência para esse array, e recebe uma referência para um inteiro, que guarda a quantidade de países
armazenados no vetor */
Pais* criarPaises(int* quantidade) {
    /* Alocação de um vetor para países. Inicialmente, possui capacidade para 50 elementos, mas essa quantidade pode ser
    aumentada utilizando a função realloc */
    int capacidade = 50;
    Pais* paises = (Pais*) malloc(sizeof(Pais) * capacidade);
    // Verificação da alocação. Caso a alocação não ocorra corretamente, 'paises' terá valor NULL
    if (!paises)
        // Retorna um ponteiro vazio, indicando erro na função
        return NULL;
    // O 'indice' serve para inserção dos paises dentro do array e também controla a quantidade de países que foram inseridos
    int indice = 0;
    // Abre o arquivo em modo leitura
    FILE* noc = fopen("noc_regions.csv", "r");
    // Verificação da abertura do arquivo. Caso a abertura não ocorra corretamente, 'noc' terá valor NULL
    if (!noc) {
        // Libera a memória alocada para o vetor de países e returna um ponteiro vazio, indicando erro na função
        free(paises);
        return NULL;
    }
    /* 'camposMax' contém o valor máximo de campos em cada linha do arquivo lido. No entanto, O que nos interessa é o campo contendo
    o código NOC de cada país, que é justamente o primeiro, então o array precisa de apenas um elemento, sendo ele, o código do país */
    int camposMax = 1;
    // 'campos' é um vetor de strings, que guarda os campos já separados
    char campos[camposMax][64];
    // 'linha' armazena uma linha do arquivo
    char linha[128];
    // Tanto 'campos', quanto 'linha' serão sobreescritos a cada linha do arquivo, tendo suas informações avaliadas
    // Esse primeiro fgets() serve apenas para ignorar o cabeçalho do arquivo, que contém informações irrelevantes
    fgets(linha, sizeof(linha), noc);
    // A função fgets() será chamada até que possua valor NULL, ou seja, até que a leitura falhe, isso indica fim do arquivo
    while (fgets(linha, sizeof(linha), noc) != NULL) {
        // Trata a string lida, substituindo a quebra de linha pelo terminador nulo, responsável por indicar o final da string
        linha[strcspn(linha, "\n")] = '\0';
        /* Aqui, a função 'separarCampos' é chamada para separar a string 'linha' em outras strings, estas serão armazenadas
        em 'campos'. Mas como precisamos apenas do primeiro campo, a função lê a string 'linha' e ao encontrar a primeira vírgula
        separa essa substring e guarda em campos[0] */
        separarCampos(linha, campos, camposMax);
        // Aqui é feita uma verificação, acionada quanto o vetor de países atingir sua capacidade máxima
        if (indice >= capacidade) {
            // A capacidade é aumentada e um novo espaço na memória é alocada, contendo os elementos de 'paises', mas agora com capaciade maior
            capacidade += 50;
            Pais* temp = realloc(paises, capacidade * sizeof(Pais));
            // Verificaçao da alocação. Caso a alocação não ocorra corretamente, 'tempo' terá valor NULL
            if (!temp) {
                // Em caso de falha na alocação, libera a memória reservada, fecha o arquivo e retorna um ponteiro vazio
                free(paises);
                fclose(noc);
                return NULL;
            }
            // Muda a refêrencia de 'países', que agora aponta para o novo vetor com capacidade maior
            paises = temp;
        }
        // Copia o conteúdo encontrado no campo lido no arquivo e atribui ele ao campo 'noc' da estrutura Pais
        strcpy(paises[indice].noc, campos[0]);
        // Atribui 0 ao número de medalhas conquistada por esse mesmo país.
        paises[indice].medalhas = 0;
        // Incrementa o índice para a próxima inserção
        indice++;
    }
    // Altera o valor apontado por 'quantidade', que passa a conter o número total de países lidos
    *quantidade = indice;
    // Retorna a referência desse vetor de países
    return paises;
}
/* Esta função é responsável por contar o número total de medalhas conquistadas por cada país em um determinado esporte.
Ela recebe a lista de países, a quantidade de países e o nome do esporte que se deseja verificar. A função percorre o 
arquivo principal e separa cada linha em seus respectivos campos, em seguida, verifica se o jogador representado na linha 
recebeu uma medalha no respectivo evento, se sim, verifica o código NOC do jogador e incrementa o número de medalhas daquele país */
void contarMedalhasPorPais(Pais* paises, int quantidade, char* esporte) {
    // Tentativa de abrir o arquivo que contém as informações sobre os eventos olímpicos
    FILE* results = fopen("results.csv", "r");
    // Se a tentativa falhar, para a execução da função
    if (!results)
        return ;
    // 'camposMax' representa o número máximo de campos que cada linha contém. No caso do arquivos "results.csv", são 10 campos
    int camposMax = 10;
    // 'campos' é um vetor de strings que guarda cada campo lido na linha em um índice diferente
    char campos[camposMax][64];
    // 'linha' armazena a linha lida no arquivo
    char linha[256];
    // fgets() usado para ignorar o cabeçalho do arquivo, a primeira linha, que contém informações irrelevantes
    fgets(linha, sizeof(linha), results);
    /* fgets() será chamado até que a leitura falhe, ou seja, o arquivo foi completamente lido. Cada linha lida
    será armazenada em 'linha', e essa variável será sobrescrita a cada leitura */
    while (fgets(linha, sizeof(linha), results) != NULL) {
        // Faz um tratamento na string, substituindo a quebra de linha pelo terminador nulo, que representa o final da string
        linha[strcspn(linha, "\n")] = '\0';
        /* Cada linha será separada em seus respectivos campos, por meio da função 'separarCampos'. Cada campo será armazenado em 
        um elemento de 'campos'.*/
        separarCampos(linha, campos, camposMax);
        // O campo que contém o esporte é o nono, portanto está armazenado no índice 8 de 'campos'
        char* campoEsporte = campos[8];
        /* 'medalhista' é uma variável booleana que verifica se o campo no índice 4 (o quinto campo no arquivo, referente a medalha)
        não está vazio, o que significa que o jogador foi medalhista nesse esporte */
        int medalhista = strlen(campos[4]) != 0;
        // Verifica se o nome do esporte recebido como argumento da função é o mesmo lido na linha do arquivo e se o jogador é medalhista
        if (strcmp(campoEsporte, esporte) == 0 && medalhista) {
            // O 8º campo representa o código NOC do jogado (e é acessado pelo índice 7 do vetor 'campos')
            char* codigoNoc = campos[7];
            // Percorre o vetor de países, sempre verificando se o código NOC dos países dentro do vetor é o mesmo lido no arquivo
            for (int i = 0; i < quantidade; i++) {
                if (strcmp(paises[i].noc, codigoNoc) == 0) {
                    // Em caso positivo, incrementa o campo 'medalhas' da estrutura
                    paises[i].medalhas++;
                }
            }
        }
    }
}
// Função de comparação para o países tendo como critério o número total de medalhas
int compararPaises(const void* primeiro, const void* segundo) {
    const Pais *p1 = primeiro;
    const Pais *p2 = segundo;
    if (p1->medalhas != p2->medalhas)
        return p2->medalhas - p1->medalhas;
    return strcmp(p1->noc, p2->noc);
}
// Função para ordenar um vetor de países pelo número de medalhas
void ordenarPorMedalhas(Pais* paises, int quantidade) {
    qsort(paises, quantidade, sizeof(Pais), compararPaises);
}


/* FUNÇÕES QUE PODEM AJUDAR!!*/


int todosOsNocsDe(char nomepais[50], char NOCs[][4]){
    /* Abrindo o arquivo que contém os dados acerca dos NOCs.*/
    FILE *arquivosnocs = fopen("noc_regions.csv", "r");
    if (arquivosnocs == NULL) {
        printf("Erro ao abrir arquivo\n");
        return -1;
    }

    char linha[1024];
    /* Ignorando o cabeçalho. */
    fgets(linha, sizeof(linha), arquivosnocs);
    int i = 0;
    while(fgets(linha, sizeof(linha), arquivosnocs) != NULL){
        /* Tratando a linha lida. */
        linha[strcspn(linha, "\n")] = '\0';
        /* Declarando uma array que irá receber os campos das linhas do arquivo cvs: */
        char campos[3][1024];
        separaLinhaCvs(linha,campos, 3);
        /* Armazenando os campos pertinentes: */
        char *NOC = campos[0];
        char *pais = campos[1];
        if(strcmp(nomepais, pais) == 0){
            strcpy(NOCs[i], NOC);
            i++;
        }
    }
    fclose(arquivosnocs);
    return i;
}

int quantasMedalhas(char nomepais[50]){
    /* Usando a função todosOsNocsDe para acessar todos os NOCs de um país
    ao longo de todas as edições. */
    char NOCs[10][4];
    int quantosNocs = todosOsNocsDe(nomepais, NOCs);
    /*Analisando, então, o arquivo de resultados das Olimpíadas, de modo a comparar o NOC dos
    atletas medalhistas com o do país escolhido. */
    FILE* results = fopen("results.csv", "r");
    /* Testando se o arquivo foi aberto com sucesso...*/
    if (results == NULL){
        printf("Erro ao abrir results\n");
        return -1;
    }

    char linha[1024];
    /* Ignorando o cabeçalho do arquivo... */
    fgets(linha, sizeof(linha), results);
    int totaldemedalhas = 0;
    while(fgets(linha, sizeof(linha), results) != NULL){
        /* Tratando a linha lida. */
        linha[strcspn(linha, "\n")] = '\0';
         /* Declarando uma array que irá receber os campos das linhas do arquivo cvs: */
        char campos[10][1024];
        separaLinhaCvs(linha, campos, 10);
        /* Armazenando os campos pertinentes: */
        char *Medal = campos[4];
        char *NOC = campos[7];
        /* Percorrendo a array de NOCs para averiguar a compatibilidade do campo NOC da linha
        com o de algum NOC do país.*/
        for(int i = 0; i < quantosNocs; i++){
            /* Se o atleta for de um NOC do país escolhido e o campo Medal não for vazio 
            (não ganhou medalha), a quantidade de medalhas do país é incrementada. */
            if (strcmp(NOC, NOCs[i]) == 0){
                if (campos[4][0] != '\0') {
                totaldemedalhas++;
                }
            }
        }
    }
    fclose(results);
    /* Retornando o total de medalhas do país. */
    return totaldemedalhas;
}