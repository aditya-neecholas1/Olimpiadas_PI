#ifndef QUESTAO1_H
#define QUESTAO1_H

/* A estrutura 'Pais' será usada para representar cada um dos países encontrada no dataset. Sendo composta pelo nome do país,
seu(s) código(s) NOC (alguns países já possuíram mais de um código NOC, daí surge a necessidade de representá-los em um vetor),
junto com o número de códigos que o país possui e, por fim, o número total de medalhas conquistada pelo país. */
typedef struct {
    char nome[64];
    char codigosNOC[10][4];
    int qtdeNOCs;
    int medalhas;
} Pais;

static int separarCampos(char* lista, char campos[][64], int camposMax);

Pais* criarPaises(int* quantidade);

void contarMedalhasPorPais(Pais* paises, int quantidade, char* esporte);

int compararPaises(const void* primeiro, const void* segundo);

void ordenarPorMedalhas(Pais* paises, int quantidade);

#endif