#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "questao4.h"

/* Os includes incluem: biblioteca padrão de entrada e saída, de manipulação de strings, biblioteca padrão e o arquivo de cabeçalho da questão*/
/*Abaixo declarei uma função para receber o array de países, quantidade de países e nome do país, dentro dela tem um loop que percorre os países cadastrados e retorna 0 
quando o nome do país e a posição são iguais. Assim evita repetição*/
int encontrarPais (Pais paises[], int numPaises, const char *nomePais){
    for (int inteiro= 0; inteiro< numPaises; inteiro++){
        if (strcmp (paises[inteiro].nome, nomePais) == 0){
            return inteiro;
        }
    }
return -1; // Indica que não encontrou o país em nenhuma posição.
}

/*Essa função abaixo vai "transformar" o arquivo para facilitar o programa, separando uma linha CSV em campos individuais*/
void transformarLinha (char *linha, char campos[][TAM_LINHA], int *numCampos){
    int inteiro=0; //Percorre cada caracter da linha
    int campoAtual=0;
    int posicaoCampo=0;
    int dentroAspas=0; // Indica se há aspas

    *numCampos =0;

    /*O loop feito abaixo vai percorrer até encontrar o fim da string "como toda string finaliza em '\0', ou uma quebra de linha, ou um retorno"*/
    while (linha[inteiro] != '\0' && linha[inteiro] != '\n' && linha[inteiro] != '\r'){
        if (linha[inteiro]=='"'){ // Se encontrar aspas, ignora ela.
            dentroAspas =! dentroAspas;
            inteiro++;
            continue;
        }
        if (linha[inteiro] ==',' && !dentroAspas){ //Se encontrou vírgula e não está dentro de aspas, significa que é separador de campo
            campos[campoAtual][posicaoCampo] = '\0';
            posicaoCampo = 0;
            inteiro++;
            continue;
        }

        campos[campoAtual][posicaoCampo] = linha[inteiro];//Copia o caractere atual para dentro do campo
        posicaoCampo++;
        inteiro++;
    }

    campos [campoAtual][posicaoCampo] = '\0'; //Finaliza o último campo com caractere nulo

    *numCampos = campoAtual + 1; //Guarda o total de campos encontrados- como o campoAtual começa em 0, então acrescenta +1
}
 /* A função abaixo basicamente processa o arquivo CSV recebendo o nome dele, o array de países e um ponteiro para o número de países. Depois //Abre o arquivo em modo 
 de leitura, guarda o ponteiro em 'arquivo', verifica se há erro, lê o cabeçalho do arquivo e ignora, e lê as linhas seguintes até o fim do arquivo, nesse caso 'NULL'. 
 */

void processarArquivo(Pais paises[], int *numPaises){ 
    FILE *arquivo = fopen ("bios.csv", "r"); 
    if (arquivo == NULL){
        PRINTF("Erro ao abrir o arquivo bios.csv\n");
        exit(1);
    }
    char linha[TAM_LINHA];
    *numPaises = 0;

    fgets (linha, TAM_LINHA, arquivo);

    while (fgets(linha, TAM_LINHA, arquivo) != NULL){
        char campos[20][TAM_LINHA];
        int numCampos = 0;

        transformarLinha(linha, campos, &numCampos);
        
        /*Como o arquivo utilizado nessa questão tem 16 campos(colunas), precisamos utilizar apenas 2 deles, que é o campo que indica o gênero (campo1) e o campo que 
        indica o país(campo6, campo do NOC), por isso identifica o numero de Campos menor do que 7*/
        if (numCampos <7){ 
            continue;
        }
        
        char *sexo = campos[1];
        char *pais = campos[6];

        if (strlen(pais) > 0 && strlen (sexo)> 0){ // Confere se os campos em que os ponteiros apontaram acima (pais e sexo) não estão vazios
            int indicePais = encontrarPais (paises, *numPaises, pais);

            if(indicePais==-1){// Confere se o país já foi cadastrado e se o limite máximo já foi atingido de países (250 países, como foi declarado no cabeçalho).
                if(*numPaises>= MAX_PAISES){
                    continue;
                }

                strcpy(paises[*numPaises].nome, pais);//Copia o nome do país para a próxima posição livre no array
                /*Começa o contador de atletas masculinos e femininos com zero e ao final incrementa o total de países cadastrados*/
                paises[*numPaises].atletasMasculinos = 0;
                paises[*numPaises].atletasFemininos = 0;
                indicePais = *numPaises;
                (*numPaises)++;
            }
            if (strcmp(sexo, "Male")==0){
                paises[indicePais].atletasMasculinos++;
            }
            else if(strcmp (sexo, "Female"==0)){
                paises[indicePais].atletasFemininos++;
            }
        }
    
        }

        fclose(arquivo);
    }
    /*Essa função foi criada para ordenar os países por quantidade de atletas femininas em ordem decrescente, então começa pelo maior, então ele vai comparando os elementos em loop
     e diminuindo já que o final está ordenado. Se a quantidade de atletas femininas atual é menor que a próxima, então troca a posição.*/
    void ordenarPaisesDecrescente (Pais paises[], int numPaises){

        for (int inteiro1 = 0 ; inteiro1 < numPaises - 1; inteiro1++) {
            for (int inteiro2 = 0; inteiro2<numPaises-inteiro1-1; inteiro2++){
                if (paises[inteiro2].atletasFemininos < paises[inteiro2 + 1]. atletasFemininos){
                     Pais temporario = paises[inteiro2];
                     paises[inteiro2] = paises[inteiro2 +1];
                     paises[inteiro2 + 1] = temporario;
                }

            }
        }
    }

    /*Essa função abaixo foi criada para filtrar, ordenar e por fim exibir os países que contém mais mulheres do que homens, verificando as condições falsas e imprimindo o resultado das verdadeiras.*/
    void listarPaises (Pais paises[], int numPaises){
        Pais paisesComMaisMulheres[MAX_PAISES];
        int total= 0;

        for (int inteiro=0; inteiro<numPaises; inteiro++){
            if(paises[inteiro].atletasFemininos> paises[inteiro].atletasMasculinos){
                paisesComMaisMulheres[total] = paises[inteiro];
                total++;
            }
        }

    /*Para melhor organização, coloquei uma ordenação crescente dos países com mais mulheres e imprime em tabela de maneira mais clara para o usuário.*/
    ordenarPaisesDescrescente(paisesComMaisMulheres, total);

    printf("PAÍSES QUE CONTÊM MAIS MULHERES DO QUE HOMENS\n");

    if (total == 0){
        printf("Não existe país com mais atletas mulheres do que homens nas olimpiadas registradas.\n");
    }
    else{
        printf("%s %s %s\n", "Pais", "Mulheres", "Homens");
    
    for (int inteiro = 0; inteiro < total; inteiro++){
        printf("%s %d %d\n", paisesComMaisMulheres[inteiro].nome, paisesComMaisMulheres[inteiro]. atletasFemininos, paisesComMaisMulheres[inteiro].atletasMasculinos);    
    }

    printf("O total de países encontrados foi: %d\n", total);

    }

    }