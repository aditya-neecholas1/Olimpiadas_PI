#include <stdio.h>
#include <string.h>
int separaLinhaCsv(char *linha, char campos[][1024], int camposmax){
    /*A variável i vai percorrer a linha inteira,
    a variável j vai percorrer os caracteres do campo (por isso é resetada ao
    seu valor inicial de 0 a cada novo campo encontrado.),
    a variável k será usada para percorrer a posição de cada campo na array
    de campos.*/
    int i = 0, j = 0, k = 0;
    /* Para que os campos fossem separados corretamente, mostrou-se necessário
    averiguar se a vírgula faz parte do campo ou se realmente é um separador do cvs.
    Desse modo, foi criado um booleano que determina se está ou não dentro de aspas,
    estando, em primeiro momento, fora destas. */
    int dentrodeaspas = 0;

    /* Enquanto a linha não for totalmente percorrida ou quando o número máximo de
    campos desejados for atingido, faça...*/
    while(linha[i] != '\0' && k < camposmax){
        /* Sempre que uma aspa é encontrada, o estado de dentro de aspas é modificado,
        simbolizando a abertura e o fechamento destas. */
        if(linha[i] == '"'){
            dentrodeaspas = !dentrodeaspas;
        } else 
        /* Se encontrar a vírgula fora das aspas, ocorre a separação, é passado para o
        próximo campo e a variável que percorre os caracteres dos campos é reiniciada. */
        if (linha[i] == ',' && !dentrodeaspas){
            campos[k][j] = '\0';
            k++;
            j = 0;
        } /* Se algo diferente de vírgula ou algo dentro de aspas é percorrido, este caractere
        é copiado como parte do campo. */
        else {
            campos[k][j++] = linha[i];
        }
        i++;
    }
    
    /* O final do último campo também recebe \0.*/
    campos[k][j] = '\0';
    /* Enfim, será retornado o número de campos encontrados. */
    return k + 1;
}