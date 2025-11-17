#include "BigInt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*==quantidade de dígitos por nó==*/
#define qtdDigitos 3
// (cada nó deve conter um bloco com k digitos)

/*===Struct que representa blocos do grande número===*/
// contem inteiro de k digitos e ponteiro para o próximo bloco
typedef struct Bloco
{
    signed int num;
    struct Bloco *previous; // aponta para o bloco de k digitos anterior
    struct Bloco *next;     // aponta para o próximo bloco de k digitos
} bloco;                    // ou nó!!

/*===Struct que representa um grande número===*/
// >Contém informações gerais sobre o grande número
// >Contém ponteiro para o bloco de números mais à direita (tail) e um para o mais à esquerda (head)
//         >Ex: se o big int é 123.930.321, tail apontaria para o bloco 321, head apontaria para 123
struct bigint
{
    bloco *head; // ponteiro para o bloco mais a esquerda
    bloco *tail; // ponteiro para bloco mais à direita

    int sinal; // Indica o sinal do Big int, +1 se positivo ou -1 se negativo
    int size;   // quantidade de blocos (nós) do big int
};

/* ===visualização da lista encadeada big int ===

    [BigInt]->[bloco]<->[bloco]<->[bloco]
               ^^tail           ^^head

    ex: 987.345.789
    [BigInt]->[789]<->[345]<->[987]
    |  |      ^                ^
    |  -------| tail           | head
    ----------------------------
*/

/*===Funções de criação===*/

BigInt *criarBigInt(char *numero){
    /*verificando se o ponteiro para a string que contém o número é valido*/
    if (!numero)
        return NULL;

    int tam = strlen(numero); // quantidade de digitos do número (tamanho da string)

    /*criando big int*/
    BigInt *n = malloc(sizeof(BigInt));
    if (!n) // verificando se a memória foi alocada
        return NULL; 

    /*==configurando o bigint criado==*/
    n->tail = NULL;
    n->size = 0;

    // verificando se o número passado é negativo ou positivo
    if (numero[0] == '-'){
        n->sinal = -1;

        /*após verificar o sinal, exclui o caracter '-', deixando apenas o número*/
        for (int j = 0; j < tam - 1; j++){
            numero[j] = numero[j + 1];
        }
        tam--;
    }
    else{
        n->sinal = 1;
    }

    int valor; // variável que vai guardar o valor de cada bloco do grande número
    int i, j;  // variáveis auxiliares

    /*O seguinte loop permite a criação dos blocos*/
    while (tam > 0){
        valor = j = 0;

        /*O i indica a posição a partir da qual deve ser lida a string*/
        /*tam indica a posição até onde a string deve ser lida*/
        // De i até o tam, os char serão convertidos em int
        // o resultado será salvo em 'valor', que será usado para compor um novo nó
        if (tam > qtdDigitos) //determinando o i com base na quantidade de chars não lidos na string
            i = tam - qtdDigitos; 
        else
            i = 0;

        /*convertendo os digitos de char para int*/
        while (i < tam) {
            valor = valor * 10 + (numero[i] - '0');
            j++; // j armazena a quantidade de digitos do valor ( = qtd de char lidos da string numero)
            i++;
        }

        
        tam = tam - j;

        // a seguinte comparação garante que não seja criado um último bloco só de 0s (desnecessário, só desperdiça memória) 
        if(tam == 0 && valor == 0 && j>=1)
             break;

        inserirBloco(n, valor);
    }

    return n;
}

void inserirBloco(BigInt *n, int valor){
    if (!n)
        return; // caso o BigInt seja inválido

    bloco *b = malloc(sizeof(bloco));
    if (!b)
        return;

    /*==configurando bloco==*/
    b->num = valor;
    b->next = NULL;

    if (n->size == 0){
        // caso seja o primeiro nó
        n->tail = b;
        b->previous = NULL; //se for o primeiro, não existe elemento anterior
    }
    else{
        bloco *tmp = n->tail;

        // percorre o BigInt até o último bloco
        for (int i = 1; i < n->size; i++){
            tmp = tmp->next;
        }

        tmp->next = b;
        b->previous = tmp;
    }
    n->head = b; // o bloco mais recente é sempre a cabeça
    n->size++;
}

void destruirBigInt(BigInt **n){
    if (!n || !(*n)) return;

    bloco *tmp = (*n)->tail;

    //percorre blocos e libera um por um
    while (tmp != NULL){
        bloco *rmv = tmp;
        tmp = tmp->next;
        free(rmv);
    }

    free(*n);
    *n = NULL;

    return;
}

/*===outras funções===*/

void imprimeBigInt(BigInt *n){
    if (!n)
        return; // verificando se ponteiro é válido

    // caso o grande número seja negativo, imprime sinal de negativo
    if (n->sinal == -1)
        printf("-");

    /*começa a imprimir pelo último nó (bloco mais significativo)*/
    bloco *tmp = n->head;
    printf("%d", tmp->num);

    for (int i = 1; i < n->size; i++){
        tmp = tmp->previous;
        printf("%03d", tmp->num);
    }

    printf("\n");
}

int comparaBigInt(BigInt *a, BigInt *b){
    if (!a || !b)
        return -1;

    // se a>0 e b<0, então a>b
    if (a->sinal == 1 && b->sinal == -1)
        return 1;
    // se a<0 e b>0, então a<b
    if (a->sinal == -1 && b->sinal == 1)
        return 2;

    // flag que indica se os números são positivos ou negativos
    // é utilizada no retorno das funções
    int sign;
    if (a->sinal == 1)
        sign = 1;
    else
        sign = 0;

    // comparando a quantidade de blocos de cada big int
    if (a->size > b->size)
        return (sign == 1) ? 1 : 2; // se a,b>0 o número com mais blocos é maior
    else if (a->size < b->size)
        return (sign == 1) ? 2 : 1; // se a,b<0 o número com menos blocos é maior

    /// se os big int tiverem a mesma qtd de blocos, é necessário percorrê-los
    int i = 0;                // variável auxiliar
    bloco *a_bloco = a->head; // utilizado para percorrer o big int a
    bloco *b_bloco = b->head; // utilizado para percorrer o big int b

    // loop que percorre os big int comparando os blocos correspondentes
    while (i < a->size)
    {
        if (a_bloco->num > b_bloco->num)
            return (sign == 1) ? 1 : 2;
        else if (a_bloco->num < b_bloco->num)
            return (sign == 1) ? 2 : 1;

        a_bloco = a_bloco->previous;
        b_bloco = b_bloco->previous;

        i++;
    }

    // se a função chegou até aqui, os números são iguais
    return 3;
}

void subtrairBigInt(BigInt *a, BigInt *b, int sizeSoma, int *i, int *somas){
    int carry = 0;
    bloco *a_bloco = a->tail;
    bloco *b_bloco = b->tail;

    while (*i < sizeSoma){
        if (!a_bloco){                       // caso o big int 'a' tenha sido completamente percorrido
            somas[*i] = b_bloco->num - carry; // apenas subtrai o carry do bloco do big int b
            b_bloco = b_bloco->next;         // avança ponteiro que percorre o big int b
        }
        else if (!b_bloco){                                    // caso o big int 'b' tenha sido completamente percorrido
            somas[*i] = a_bloco->num - carry; // apenas subtrai o carry do bloco do big int a
            a_bloco = a_bloco->next;         // avança ponteiro que percorre o big int a
        }
        else {
            somas[*i] = a_bloco->num - b_bloco->num - carry; // subtrai os blocos correspondentes e o carry
            a_bloco = a_bloco->next;                        // avança ponteiro que percorre o big int a
            b_bloco = b_bloco->next;                        // avança ponteiro que percorre o big int b
        }

        // verificando se houve carry
        if (somas[*i] < 0) {
            carry = 1;
            somas[*i] = 1000 + somas[*i]; //equivalente a fazer somas[*i] % 1000 (mas o operador % não funciona bem com negativos)
        }
        else {
            carry = 0;
        }


        (*i)++; // índice avança para o próximo bloco
        }

    (*i)--; // para que aponte para a última posição preenchida do vetor somas
}

BigInt *somarBigInt(BigInt *a, BigInt *b){
    // verifica se ponteiros são válidos
    if (!a || !b)
        return NULL;

    int ehNegativo = 0; //flag que indica se a soma é negativa (1) ou não (0)
    int carry = 0;  // flag de carry, 0 se não houver carry, 1 se houver
    int i = 0;      // variável auxiliar, vai percorrer vetor somas
    int somas[100]; // vetor cujos elementos são as somas de cada bloco
    char soma[100]; // string que irá guardar a soma dos grandes números

    int sizeSoma = (a->size > b->size) ? a->size : b->size; // guarda a maior qtd de blocos entre 'a' e 'b'
    bloco *a_bloco = a->tail;  // variavel usada para percorrer o big int A
    bloco *b_bloco = b->tail;  // variavel usada para percorrer o big int B


    // caso 1: números de mesmo sinal, basta somar os números e atribuir o mesmo sinal ao resultado
    if (a->sinal == b->sinal){

        if(a->sinal == -1) ehNegativo = 1;
        
        /*O seguinte loop percorre os grandes números bloco por bloco e soma os blocos correspondentes*/
        while (i < sizeSoma){

            if (!a_bloco){ // caso o big int 'a' tenha sido completamente percorrido
                somas[i] = b_bloco->num + carry; // soma apenas o numero no bloco do big int b e o carry
                b_bloco = b_bloco->next;         // avança ponteiro que percorre o big int b
            }
            else if (!b_bloco){ // caso o big int 'b' tenha sido completamente percorrido
                somas[i] = a_bloco->num + carry; // soma apenas o numero no bloco do big int a e o carry
                a_bloco = a_bloco->next;         // avança ponteiro que percorre o big int a
            }
            else{
                somas[i] = a_bloco->num + b_bloco->num + carry; // somando os blocos correspondentes e o carry
                a_bloco = a_bloco->next;                        // avança ponteiro que percorre o big int a
                b_bloco = b_bloco->next;                        // avança ponteiro que percorre o big int b
            }

            // verificando se houve carry (soma extrapolou a qtdDigitos)
            if (somas[i] >= 1000){
                carry = 1;
                somas[i] = somas[i] % 1000;
            }
            else {
                carry = 0;
            }

            i++; // índice avança para o próximo bloco
        }

        // verifica se houve carry na soma do último bloco
        if (carry)
            somas[i] = 1; // se sim, passa o carry para o próximo bloco (posição no vetor somas, antes vazia)
        else
            i--; // se não, diminui o i para que ele aponte para a última posição preenchida do vetor somas

    } 
    else{  //caso os sinais sejam diferentes
        //é necessário descobrir qual número é maior em módulo para facilitar a subtração
        /*salvando o sinal original*/
        int aSinal = a->sinal;

        // para poder comparar os módulos
        a->sinal = 1;
        b->sinal = 1;

        int flagComp = comparaBigInt(a, b); //guarda a informação da comparação entre |a| e |b|

        if(flagComp == 3){ // |a| = |b|, portanto a+b = 0;
            somas[i] = 0;
        }
        else if(flagComp == 1){ //se |a| > |b|

            if (aSinal == -1){ //se a < 0
                ehNegativo = 1; //a < 0 (sendo b > 0) e |a| > |b|, entao a+b < 0
            }
            else{ // b < 0
                ehNegativo = 0; //b < 0 (sendo a > 0) e |a| > |b|, entao a+b > 0
            }
            //subtrai |a| - |b|
            subtrairBigInt(a, b, sizeSoma, &i, somas);
        }
        else{ // se |b| > |a|
            if (aSinal == -1){
                ehNegativo = 0; //a < 0 (sendo b > 0) e |b| > |a|, então a+b > 0
            } else{ // b < 0
                ehNegativo = 1;  //b < 0 (sendo a > 0) e |b| > |a|, então a+b < 0
            }

            //subtrai |b| - |a|
            subtrairBigInt(b, a, sizeSoma, &i, somas);
        }
    }

    /*transformando o vetor somas em uma string*/
        if (ehNegativo){
            sprintf(soma, "%c", '-'); // se o sinal for negativo, inicializa a string com '-'
        } else{
            sprintf(soma, "%d", somas[i]); // se o sinal for positivo, inicializa a string soma com os primeiros digitos da soma
            i--;
        }
        // essa função percorre o vetor somas de trás para frente, concatenando os seus elementos a string soma
        while (i >= 0){
            char tmp[5];                    // variável auxiliar
            sprintf(tmp, "%03d", somas[i]); // coloca inteiro contido no vetor somas na variável auxiliar
            strcat(soma, tmp);
            i--;
        }

    return criarBigInt(soma);
}
