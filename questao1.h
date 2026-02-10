#ifndef QUESTAO1_H
#define QUESTAO1_H

#ifdef _WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #define POPEN popen
    #define PCLOSE pclose
#endif

/* A estrutura 'Pais' será usada para representar cada um dos países encontrada no dataset. Sendo composta pelo nome do país,
seu(s) código(s) NOC (alguns países já possuíram mais de um código NOC, daí surge a necessidade de representá-los em um vetor),
junto com o número de códigos que o país possui e, por fim, o número total de medalhas conquistada pelo país. */
typedef struct {
    char nome[64];
    char codigosNOC[10][4];
    int qtdeNOCs;
    int medalhas;
} Pais;

typedef struct {
    char evento[128];
    char esporte[64];
    char noc[4];
    char tipo[8];
} Medalha;

void limparBuffer();

Pais* criarPaises(int* quantidade);

int medalhaJaContada(Medalha* contadas, int qtdeContadas, Medalha medalha);

Medalha criarMedalha(char* jogos, char* evento, char* tipo, char* noc, char* esporte);

void contarMedalhasPorPais(Pais* paises, int quantidade, char* esporte);

int compararPaises(const void* primeiro, const void* segundo);

void ordenarPorMedalhas(Pais* paises, int quantidade);

void desenharGrafico(Pais* paises, char* esporte);

void questao1exe();

#endif