#ifndef QUESTAO2_H
#define QUESTAO2_H

typedef struct {
    char noc[50];
    int atletas;
} PaisAtletas;

int separarCampos(char* lista, char campos[][64], int camposMax);

void contarAtletasPorPais(PaisAtletas* paises, int quantidade);

PaisAtletas* criarPaises(int *quantidade);

int compararPaises(const void* primeiro, const void* segundo);

void ordenarPorAtletas(PaisAtletas* paises, int quantidade);

#endif