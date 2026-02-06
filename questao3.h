#ifndef QUESTAO3_H
#define QUESTAO3_H

/* A estrutura 'Atelta' será usada para armazenar as informações pertinentes de uma atleta.
Neste caso, serão o seu athlete_id e suas medalhas.*/
typedef struct {
    char nome[50];
    char athlete_id[50];
    int medalhaOuro;
    int medalhaPrata;
    int medalhaBronze;
} Atleta;

int separarCampos(char* lista, char campos[][64], int camposMax);

int jaEstaNaArray(Atleta* medalhistas, char *ID, int tamanhodaarrray);

int comparaAtletas(const void *atletaA, const void *atletaB);

Atleta* todosAtletasMedalhistas(char *esporteescolhido);

#endif