#ifndef QUESTAO1_H
#define QUESTAO1_H

typedef struct {
    char noc[4];
    int medalhas;
} Pais;

int separarCampos(char* lista, char campos[][64], int camposMax);

Pais* criarPaises(int* quantidade);

void contarMedalhasPorPais(Pais* paises, char* esporte);

int compararPaises(const void* primeiro, const void* segundo);

void ordenarPorMedalhas(Pais* paises, int quantidade);

#endif