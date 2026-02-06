#ifndef QUESTÃO4_H // Para definir a seguir a questão 4
#define QUESTAO4_H

// Definindo o valor das constantes prescritas abaixo. 
#define MAX_PAISES 250
#define MAX_NOME 100
#define TAM_LINHA 2000

//Aqui eu defini uma struct que armazene dados de um certo país
typedef struct {
    char nome [MAX_NOME];
    int atletasFemininos;
    int atletasMasculinos;
} Pais;

//Criando um 'protótipo' das funções utilizadas no arquivo de implementação abaixo

void processarArquivo (const char *nomeArquivo, Pais paises[], int *numPaises);
void ordenarPaisesDecrescente(Pais paises[], int numPaises);
void listarPaises(Pais paises[], int numPaises);
int encontrarPais(Pais paises[], int numPaises, const char *nomePais);

#endif



