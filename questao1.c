#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "questao1.h"

void limparBuffer() {
    int limpeza;
    while((limpeza = getchar()) != '\n' && limpeza != EOF);
}
// Para um esporte escolhido, mostre o ranking dos países com mais medalhas nesse esporte.
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
/* Função responsável por criar um array contendo todos os países presente no arquivo "noc_regions". 
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
    FILE* arquivo = fopen("noc_regions.csv", "r");
    // Verificação da abertura do arquivo. Caso a abertura não ocorra corretamente, 'arquivo' terá valor NULL
    if (!arquivo) {
        // Libera a memória alocada para o vetor de países e returna um ponteiro vazio, indicando erro na função
        free(paises);
        return NULL;
    }
    /* 'camposMax' contém o valor máximo de campos em cada linha do arquivo lido. No entanto, O que nos interessa é o campo contendo
    o código NOC de cada país, que é o primeiro, e o nome do país, presente no segundo campos. Então o array precisa de apenas dois elementos */
    int camposMax = 2;
    // 'campos' é um vetor de strings, que guarda os campos já separados
    char campos[camposMax][64];
    // 'linha' armazena uma linha do arquivo
    char linha[128];
    // Tanto 'campos', quanto 'linha' serão sobreescritos a cada leitura de linha do arquivo, tendo suas informações avaliadas
    // Esse primeiro fgets() serve apenas para ignorar o cabeçalho do arquivo, que contém informações irrelevantes
    fgets(linha, sizeof(linha), arquivo);
    // A função fgets() será chamada até que possua valor NULL, ou seja, até que a leitura falhe, indicando fim do arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Trata a string lida, substituindo a quebra de linha pelo terminador nulo, responsável por indicar o final da string
        linha[strcspn(linha, "\n")] = '\0';
        /* Aqui, a função 'separarCampos' é chamada para separar a string 'linha' em substrings, estas serão armazenadas em 'campos' */
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
                fclose(arquivo);
                return NULL;
            }
            // Muda a refêrencia de 'países', que agora aponta para o novo vetor com capacidade maior
            paises = temp;
        }
        // O valor do primeiro campo da linha lida (acessado por campos[0]) corresponde ao código NOC país
        char* noc = campos[0];
        // O valor do segundo campo da linha lida (acessado por campos[1]) corresponde ao nome do país
        char* nome = campos[1];
        // A variável booleana 'paisJaExistente' é responsável por indicar se o país encontrado já foi inserido no array de países
        int paisJaExistente = 0;
        // Percorre por todos os países já inseridos, verificando se um país com o mesmo nome que aquele lido na linha do arquivo já está presente no array
        for (int i = 0; i < indice; i++) {
            if (strcmp(paises[i].nome, nome) == 0) {
                /* Caso o país já tenha sido inserido no array, atribui o valor do campo NOC a um dos elementos do vetor de códigos daquele país. 
                O campo 'qtdeNOCs' presente na estrutura 'Pais' faz o controle desse vetor */
                strcpy(paises[i].codigosNOC[paises[i].qtdeNOCs], noc);
                paises[i].qtdeNOCs++;
                // A variável booleana recebe valor 1, indicando que o país já existe no array de países
                paisJaExistente = 1;
            }
        }
        // Caso o país lido no arquivo ainda não tenha sido inserido no array de países, agora será
        if (!paisJaExistente) {
            // Atribui o nome lido na linha do arquivo ao campo 'nome' do novo país
            strcpy(paises[indice].nome, nome);
            // Copia o conteúdo encontrado no campo lido no arquivo e atribui ele ao primeiro elemento do vetor 'codigosNoc' da estrutura Pais
            strcpy(paises[indice].codigosNOC[0], noc);
            // O pais começa tendo um código NOC
            paises[indice].qtdeNOCs = 1;
            // Atribui 0 ao número de medalhas conquistada por esse mesmo país.
            paises[indice].medalhas = 0;
            // Incrementa o índice para a próxima inserção
            indice++;
        }
    }
    // Após o fim da leitura, fecha o arquivo
    fclose(arquivo);
    // Altera o valor apontado por 'quantidade', que passa a conter o número total de países lidos
    *quantidade = indice;
    // Retorna a referência desse vetor de países
    return paises;
}
// Função que devolve um valor booleano que representa se a medalha analisada já está incluída em um array de medalhas
int medalhaJaContada(Medalha* contadas, int qtdeContadas, Medalha medalha){
    for (int i = 0; i < qtdeContadas; i++) {
        if (!strcmp(contadas[i].evento, medalha.evento) && !strcmp(contadas[i].noc, medalha.noc) && !strcmp(contadas[i].esporte, medalha.esporte) && !strcmp(contadas[i].tipo, medalha.tipo))
            return 1;
    }
    return 0;
}
// Função que cria uma medalha, atribuindo aos seus campos os argumentos recebidos
Medalha criarMedalha(char* jogos, char* evento, char* tipo, char* noc, char* esporte) {
    Medalha medalha;
    strcpy(medalha.evento, jogos);
    strcat(medalha.evento, evento);
    strcpy(medalha.tipo, tipo);
    strcpy(medalha.noc, noc);
    strcpy(medalha.esporte, esporte);
    return medalha;
}

/* Esta função é responsável por contar o número total de medalhas conquistadas por cada país em um determinado esporte.
Ela recebe a lista de países, a quantidade de países e o nome do esporte que se deseja verificar. A função percorre o 
arquivo principal e separa cada linha em seus respectivos campos, em seguida, verifica se o jogador representado na linha 
recebeu uma medalha no respectivo evento, se sim, verifica o código NOC do jogador e incrementa o número de medalhas daquele país */
void contarMedalhasPorPais(Pais* paises, int qtdePaises, char* esporte) {
    // Variáveis para representar a capacidade do array de medalhas contadas e a quantidade de medalhas que já foram inseridas (também serve para fazer o controle de inserção)
    int capacidadeMedalhas = 100, qtdeContadas = 0;
    // Array de medalhas que representa as medalhas que já foram contadas (Essa abordagem busca resolver um problema onde esportes coletivos influenciam muito mais no total de medalhas
    // que um país conquistou, isso porque cada atleta recebe uma medalha)
    Medalha* contadas = malloc(sizeof(Medalha) * capacidadeMedalhas);
    // Tentativa de abrir o arquivo que contém as informações sobre os eventos olímpicos
    FILE* arquivo = fopen("results.csv", "r");
    // Se a tentativa falhar, para a execução da função
    if (!arquivo)
        return;
    // 'camposMax' representa o número máximo de campos que cada linha contém. No caso do arquivos "results.csv", são 10 campos
    int camposMax = 10;
    // 'campos' é um vetor de strings que guarda cada campo lido na linha em um índice diferente
    char campos[camposMax][64];
    // 'linha' armazena a linha lida no arquivo
    char linha[256];
    // fgets() usado para ignorar o cabeçalho do arquivo, a primeira linha, que contém informações irrelevantes
    fgets(linha, sizeof(linha), arquivo);
    /* fgets() será chamado até que a leitura falhe, ou seja, o arquivo foi completamente lido. Cada linha lida
    será armazenada em 'linha', e essa variável será sobrescrita a cada leitura */
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Faz um tratamento na string, substituindo a quebra de linha pelo terminador nulo, que representa o final da string
        linha[strcspn(linha, "\n")] = '\0';
        /* Cada linha será separada em seus respectivos campos, por meio da função 'separarCampos'. Cada campo será armazenado em 
        um elemento de 'campos'.*/
        separarCampos(linha, campos, camposMax);
        // O campo lido que contém o esporte é o nono, portanto está armazenado no índice 8 de 'campos'
        char* campoEsporte = campos[8];
        /* 'medalhista' é uma variável booleana que verifica se o campo no índice 4 (o quinto campo no arquivo, referente a medalha)
        não está vazio, o que significa que o jogador foi medalhista nesse esporte */
        int medalhista = strlen(campos[4]) != 0;
        // Verifica se o nome do esporte recebido como argumento da função é o mesmo lido na linha do arquivo e se o jogador é medalhista
        if (strcmp(campoEsporte, esporte) == 0 && medalhista) {
            // O 8º campo representa o código NOC do jogador (e é acessado pelo índice 7 do vetor 'campos')
            char* codigoNoc = campos[7];
            // O 5º campo representa o tipo da medalha conquistada (e é acessado pelo índice 4 do vetor 'campos')
            char* tipoMedalha = campos[4];
            // O 1º campo representa em qual jogos olímpicos esse evento aconteceu (e é acessado pelo índice 0 do vetor 'campos')
            char* jogos = campos[0];
            // O 2º campo representa o evento (e é acessado pelo índice 1 do vetor 'campos')
            char* evento = campos[1];
            // Cria uma medalha com as informações lidas
            Medalha medalha = criarMedalha(jogos, evento, tipoMedalha, codigoNoc, esporte);
            // Verifica se essa medalha já foi contada anteriormente
            if (medalhaJaContada(contadas, qtdeContadas, medalha))
                continue;
            // Faz a realocação do vetor de medalhas contadas caso a capacidade máxima tenha sido atingida
            if (qtdeContadas >= capacidadeMedalhas) {
                capacidadeMedalhas += 100;
                Medalha* temp = realloc(contadas, capacidadeMedalhas * sizeof(Medalha));
                if (!temp) {
                    free(contadas);
                    fclose(arquivo);
                    return ;
                }
                contadas = temp;
            }
            // Adiciona a medalha ao array de medalhas contadas e incrementa a variável de quantidade 
            contadas[qtdeContadas++] = medalha;
            // Percorre o vetor de países, sempre verificando se o código NOC dos países dentro do vetor é o mesmo lido no arquivo
            for (int i = 0; i < qtdePaises; i++) {
                // Aqui, um segundo laço é necessário, já que alguns países possuem mais que um código NOC
                for (int j = 0; j < paises[i].qtdeNOCs; j++) {
                    if (strcmp(paises[i].codigosNOC[j], codigoNoc) == 0) {
                        // Em caso positivo, incrementa o campo 'medalhas' da estrutura
                        paises[i].medalhas++;
                    }
                }
            }
        }
    }
    // Libera a memória ocupada pelo array de medalhas
    free(contadas);
}
/* Função de comparação para o países tendo como critério o número total de medalhas. A função será usada em
um qsort(), portanto, deve receber dois ponteiros genéricos */ 
int compararPaises(const void* primeiro, const void* segundo) {
    // Converte os ponteiros para o tipo 'Pais'
    const Pais *p1 = primeiro;
    const Pais *p2 = segundo;
    // Caso o número de medalhas conquistadas por cada país for diferente, compara o esse número
    if (p1->medalhas != p2->medalhas)
        return p2->medalhas - p1->medalhas;
    // Em caso de empate, compara os países pelo nome (ordem lexicográfica)
    return strcmp(p1->nome, p2->nome);
}
// Função para ordenar um vetor de países pelo número de medalhas
void ordenarPorMedalhas(Pais* paises, int quantidade) {
    // O qsort() altera um vetor, ordenando-o por meio de uma função de comparação
    qsort(paises, quantidade, sizeof(Pais), compararPaises);
}
// Função que desenha o gráfico da questão, exibindo o resultado em forma de um gráfico de barras
void desenharGrafico(Pais* paises, char* esporte) {
    // Crie um arquivo que receberá as informações necessárias para a construção do gráfico
    FILE* dados = fopen("dados_grafico.dat", "w");
    // Verifica se o arquivo foi criado corretamente
    if (!dados) {
        printf("Erro ao criar arquivo de dados do grafico.\n");
        return;
    }
    // Percorre os 10 primeiros elementos da lista de países já ordenada e escreve os nomes e número de medalhas no arquivo
    for (int i = 0; i < 10; i++) {
        fprintf(dados, "\"%s\" %d\n", paises[i].nome, paises[i].medalhas);
    }
    // Fecha o arquivo 
    fclose(dados);
    /* Abre um processo do gnuplot e cria um canal de comunicação, permitindo enviar comandos diretamente para ele via fprintf */
    FILE* gp = POPEN("gnuplot", "w");
    // Verifica se o gnuplot foi aberto corretamente
    if (!gp) {
        printf("Erro ao abrir o gnuplot.\n");
        return;
    }
    // Configurações do gráfico (Tamanho de fonte, textos, alinhamentos etc.)
    fprintf(gp, "set terminal wxt size 900,600\n");
    fprintf(gp, "set title 'Top 10 países com mais medalhas em %s' font ',16'\n", esporte);
    fprintf(gp, "set xlabel 'Países' font ',14'\n");
    fprintf(gp, "set ylabel 'Número de medalhas' font ',14'\n");
    fprintf(gp, "set style data histograms\n");
    fprintf(gp, "set style fill solid 1 border -1\n");
    fprintf(gp, "set boxwidth 2\n");
    fprintf(gp, "set grid ytics\n");
    fprintf(gp, "set yrange [0:*]; set offsets 0, 0, 1, 0\n");
    fprintf(gp, "set xtics rotate by -30\n");
    fprintf(gp,
        "plot 'dados_grafico.dat' using 2:xtic(1) "
        "lc rgb 'blue' title 'Total de medalhas', "
        "'dados_grafico.dat' using 0:2:2 "
        "with labels offset 0,0.5 notitle\n"
    );
    // Força o envio dos comandos ao gnuplot, garantindo o funcionamento adequado
    fflush(gp);
    // Mantém o gráfico aberto até que a tecla Enter seja pressionada
    printf("\nGrafico aberto. Pressione ENTER para fechar...\n");
    getchar();
    // Fecha o gnuplot
    PCLOSE(gp);
}
void questao1exe(){
    // 1º questão: Para um esporte escolhido, mostre o ranking dos países com mais medalhas nesse esporte.
    limparBuffer();
    // Leitura do esporte desejado
    printf("Digite o nome do esporte que deseja consultar\n> ");
    char esporte[32];
    fgets(esporte, sizeof(esporte), stdin);
    esporte[strcspn(esporte, "\n")] = '\0';
    // Criação da lista de países, 'quantidade' guardará o número total de países
    int quantidade;
    Pais* paises = criarPaises(&quantidade);
    // Com a lista contendo todos os países, agora as medalhas serão contabilizadas
    contarMedalhasPorPais(paises, quantidade, esporte);
    // Ordena a lista por quantidade de medalhas
    ordenarPorMedalhas(paises, quantidade);
    // Verifica se algo deu errado no processo
    if (paises[0].medalhas == 0) {
        printf("Esse esporte não existe, parça.\n");
        return;
    }
    // Exibe o resultado no terminal
    printf("RANKING DOS PAISES QUE MAIS RECEBEM MEDALHAS EM: %s\n", esporte);
    for (int i = 0; i < 10; i++) {
        printf("%dº - %s: %d medalhas\n", i + 1, (paises + i)->nome, (paises + i)->medalhas);
    }
    // Mostra o gráfico do resultado
    desenharGrafico(paises, esporte);
    // Libera o espaço da lista de países
    free(paises);
    // Finaliza a execeução da questão
    printf("Pressione Enter para voltar.");
    getchar();
}