#include <stdio.h>
#include <stdlib.h>
#include "BigInt.h"
#include <string.h>

int main(){
    int n; //representa a quantidade de operações a serem realizadas
    scanf("%d", &n);

    char op[6]; //string receberá a operação
    char num1[256], num2[256]; //strings que receberão os grandes números
    for (int i = 0; i < n; i++){
        scanf("%s %s %s", op, num1, num2);

        /*criando os grandes números*/
        BigInt *n1 = criarBigInt(num1);
        BigInt *n2 = criarBigInt(num2);


        printf("Resultado :: ");

        if (!strcmp(op, "soma")){

            BigInt *soma = somarBigInt(n1, n2); //criando BigInt para a soma de n1 e n2
            imprimeBigInt(soma); //imprimindo soma

            destruirBigInt(&soma);
        }
        else{
            int flagComp = comparaBigInt(n1, n2);

            if (!strcmp(op, "maior")){ //se o usuário quiser saber a veracidade de n1>n2
                if (flagComp == 1)
                    printf("True\n");
                else
                    printf("False\n");                 
            }
            else if (!strcmp(op, "menor")){ //se o usuário quiser saber a veracidade de n1<n2
                if (flagComp == 2)
                    printf("True\n");
                else
                    printf("False\n");
            }
            else if (flagComp == 3) //a última possibilidade é o usuário querer saber se n1 = n2
                    printf("True\n");
                else
                    printf("False\n");
        }

        destruirBigInt(&n1);
        destruirBigInt(&n2);
    }
}