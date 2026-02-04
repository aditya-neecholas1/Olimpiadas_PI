/*Questão: 12) Considere 10 países a sua escolha. Crie um ranking dos países com base no número total de medalhas
conquistadas. Ordene os países pelo total de medalhas

LOGICA: 1- Escolhe 10 países;
2-Conta total de medalhas de cada um deles e rankeia isso na impressao;
3- Ordena países de maneira decrescente do que possui maior numero de medalhas para o que contém menor numero e imprime;
4- Gráfico (EXTRA).
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 2048
#define MAX_PAISES 250
#define MAX_NOC 4
#define MAX_NOME_PAIS 100

//Para armazenar os dados de madalha por país criei uma struct:
typedef struct{
char noc[MAX_NOC]; //Aqui declarei a variavel do codigo do país.
char nomePais[MAX_NOME_PAIS]; // Aqui é a variavel para o nome completo do país.
int ouro, prata, bronze;
int total;
} Pais;

// Para mapear o NOC de acordo com o nome do país criei outra struct:
typedef struct {
    char noc[MAX_NOC];
    char nome[MAX_NOME_PAIS];
} mapeamentoNOC;

/*
.
.
.
FALTA UMA FUNÇÃO PARA REMOVER ESPAÇOS EM BRANCO NO INICIO E NO FIM !!!!
.
.
.
*/

// Para remover as aspas de uma string utilizei essa função:
void removeAspas(char *str) {
    char temporario[MAX_LINE];
    int inteiro1 = 0, inteiro2 = 0;
    
    while(str[inteiro1]) {
        if(str[inteiro1] != '"') {
            temporario[inteiro2++] = str[inteiro2];
        }
        inteiro1++;
    }
    temporario[inteiro2] = '\0';
    strcpy(str, temporario);
}

/*
.
.
.
IREI COLOCAR OUTRAS FUNÇÕES
.
.
.
*/

int main(){
    Pais paises[10];
    //MapeamentoNOC mapeamento[MAX_PAISES]; // FUNCAO A SER CRIADA!!
    int total_paises;
    int total_mapeamento;

    // Listando paises selecionados para utilizar 
    const char *paises_selecionados[] = {
        "BRA", "USA", "CHN", "GBR", "FRA", 
        "GER", "JPN", "AUS", "ITA", "CAN"
    };
    int num_selecionados = 10;

}
