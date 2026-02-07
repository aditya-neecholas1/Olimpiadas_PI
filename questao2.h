#ifndef QUESTAO2_H
#define QUESTAO2_H

typedef struct {
    char noc[50];
    int atletas;
} PaisAtletas;

static int separarCampos(char* lista, char campos[][64], int camposMax);

void contarAtletasPorPais(PaisAtletas* paises, int quantidade);

PaisAtletas* criarPaisesAtletas(int *quantidade);

int compararPaisesAtletas(const void* primeiro, const void* segundo);

void ordenarPorAtletas(PaisAtletas* paises, int quantidade);

#endif