#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "questao2.h"

/* O Gnuplot precisa ser aberto por meio de um "Pipe", um tubo de comunicação
   As funções para criar esse tubo têm sua nomeação variável a depender do
   do Sistema Operacional.
*/
#ifdef _WIN32 // Se for Windows
    #define POPEN _popen
    #define PCLOSE _pclose
#else // Se for outro SO
    #define POPEN popen
    #define PCLOSE pclose
#endif

static void limparBuffer(){
    int limpeza;
    while((limpeza = getchar()) != '\n' && limpeza != EOF);
}

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
        fgets(paisesEscolhidos[i].noc, 50, stdin); // Leitura dos nomes
        // Remoção do '\n' capturado pelo fgets
        paisesEscolhidos[i].noc[strcspn(paisesEscolhidos[i].noc, "\n")] = '\0';
        // Ocorreu a mudança do scanf pelo fgets para que países como "United States" sejam corretamente lidos
        paisesEscolhidos[i].atletas = 0;
    }
    *quantidade = 10; // Atualiza a variável de controle da quantidade (por padrão é 10)
    return paisesEscolhidos;
}

// Lê o arquivo bios.csv e realiza a contagem de atletas por país escolhidos previamente
void contarAtletasPorPais(PaisAtletas* paises, int quantidade){
    FILE* arquivo = fopen("bios.csv", "r");
    if(!arquivo) return; // Checa se a leitura foi realizada corretamente
    char linha[1000]; // Leitura de linhas do arquivo
    char campos[10][64]; // Matriz para guardar os campos após separação
    fgets(linha, sizeof(linha), arquivo); // Descarta o cabeçalho
    // Loop de leitura do arquivo
    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        separarCampos(linha, campos, 10);
        // O nome do país/NOC está na coluna 6 do arquivo
        char* paisLido = campos[6];
        for(int i = 0; i < quantidade; i++){
            /* O strstr erifica se o nome digitado está contido no nome,
            prevenindo que casos como "Russia Federation" não sejam identificados
            */
            if(strstr(paisLido, paises[i].noc) != NULL){
                paises[i].atletas++;
                break; // Caso encontre, não precisa testar para os outros países
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

void gerarGraficoGnuPlot(PaisAtletas* paises, int quantidade){
    // Criação de um arquivo de dados que possa ser lido pelo Gnuplot
    FILE *dados = fopen("dados.dat", "w");
   // Checa se a abertura foi feita de forma correta 
    if(!dados){
        printf("Erro ao criar arquivo de dados temporário.\n");
        return;
    }
    for(int i = 0; i < quantidade; i++){
        // Escreve "Nome do País" NumerodeAtletas
        // As aspas (\"%s\") são cruciais para lidar com países de nome composto
        fprintf(dados, "\"%s\" %d\n", paises[i].noc, paises[i].atletas);
    }
    fclose(dados);
    // Abertura do gnuplot através de um Pipe
    // A flag '-persistent' impede o fechamento instantâneo da janela ao término do programa .c
    FILE *gnuPlotPipe = POPEN("gnuplot -persistent", "w");
    // Envio de comandos para a confeccção do gráfico (como se estivéssemos no terminal do Gnuplot)
    fprintf(gnuPlotPipe, "set title 'Ranking de número de atletas por país'\n"); // Título
    fprintf(gnuPlotPipe, "set style data histograms\n"); // Estilo de representação do gráfico
    fprintf(gnuPlotPipe, "set style fill solid 1.0 border -1\n"); // Cor sólida
    fprintf(gnuPlotPipe, "set boxwidth 0.7\n"); // Largura da barra
    fprintf(gnuPlotPipe, "set xtics rotate by -45\n"); // Gira os nomes para que caibam
    fprintf(gnuPlotPipe, "set grid ytics\n"); // Linhas de grade
    fprintf(gnuPlotPipe, "set ylabel 'Número de Atletas'\n");
    // Comando final que desenha
    fprintf(gnuPlotPipe, "plot 'dados.dat' using 2:xtic(1) title 'Total de Atletas' lc rgb '#4169E1'\n");
    PCLOSE(gnuPlotPipe); // Fecha o canal e libera o Gnuplot
}

void questao2exe(){
    /*2ª questão: Considere 10 países a sua escolha. 
    Crie uma lista de países ordenada pelo número total de atletas
    cadastrados, do maior para o menor.*/
    limparBuffer();
    int quantidade;
    // Chama a função que pede os nomes e aloca memória
    PaisAtletas* paisesEscolhidos = criarPaisesAtletas(&quantidade);
    // Lê o arquivo e realiza a contagem
    contarAtletasPorPais(paisesEscolhidos, 10);
    // Verificamos se algum país possui 0 atletas, o que indica digitação incorreta
    for(int i = 0; i < 10; i++){
        if(paisesEscolhidos[i].atletas == 0){
            printf("O pais %s não possui atletas cadastrados, por favor verifique a grafia.\n", paisesEscolhidos[i].noc);
        }
    }
    // Ordena o ranking dos 10 países
    ordenarPorAtletas(paisesEscolhidos, 10);
    // Mostra a lista no terminal
    printf("RANKING DOS 10 PAISES COM MAIS ATLETAS:\n");
    for (int i = 0; i < 10; i++) {
        printf("%dº - %s: %d atletas cadastrados\n", i + 1, (paisesEscolhidos + i)->noc, (paisesEscolhidos + i)->atletas);
    }
    //  Chama a função que abre o Gnuplot
    printf("Gráfico via Gnuplot sendo gerado em outra janela...");
    gerarGraficoGnuPlot(paisesEscolhidos, 10);
    // Devolve a memória alocada
    free(paisesEscolhidos);
    // Fim da atividade
    printf("Pressione Enter para voltar.");
    getchar();
}
