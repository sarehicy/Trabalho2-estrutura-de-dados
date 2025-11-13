#include <stdio.h>
#include <stdlib.h>
#include "BigInt.h"

int main(){
    int n; //representa a quantidade de operações a serem realizadas
    scanf("%d", &n);

    char op[6]; //string receberá a operação
    char num1[100], num2[100]; //strings que receberão os grandes números
    for (int i = 0; i < n; i++){
        scanf("%s %s %s", op, num1, num2);

        /*criando os grandes números*/
        BigInt *n1 = criarBigInt(num1);
        BigInt *n2 = criarBigInt(num2);

        /*
            aqui ele verifica que operação tem q fazer e executa ela
        */

        /*teste*/
        imprimeBigInt(n1);
        imprimeBigInt(n2);

        
        /*desalocando memória*/
        free(n1);
        free(n2);
    }
}