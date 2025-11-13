#include "BigInt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*==quantidade de dígitos por nó==*/
#define qtdDigitos 3
// (cada nó deve conter um bloco com k digitos)

/*===Struct que representa blocos do grande número===*/
//contem inteiro de k digitos e ponteiro para o próximo bloco
typedef struct Bloco{
    int num;
    struct Bloco *previous; //aponta para o bloco de k digitos anterior
    struct Bloco *next; //aponta para o próximo bloco de k digitos
}bloco; //ou nó!!
            
/*===Struct que representa um grande número===*/
// >Contém informações gerais sobre o grande número 
// >Contém ponteiro para o bloco de números mais à direita (tail) e um para o mais à esquerda (head) 
//         >Ex: se o big int é 123.930.321, tail apontaria para o bloco 321, head apontaria para 123
struct bigint{
    bloco *head; //ponteiro para o bloco mais a esquerda
    bloco *tail; //ponteiro para bloco mais à direita 

    char sinal; //Indica o sinal do Big int, +1 se positivo ou -1 se negativo
    int size; //quantidade de blocos (nós) do big int
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
    if (!numero) return NULL;

    int tam = strlen(numero); //quantidade de digitos do número (tamanho da string)

    /*criando big int*/
    BigInt *n = malloc(sizeof(BigInt));
    if (!n) return NULL; //verificando se o big int foi criado 

    /*==configurando o bigint criado==*/
    n->tail = NULL;
    n->size = 0;

    //verificando se o número passado é negativo ou positivo
    if (numero[0] == '-') {
        n->sinal = -1; 

        /*após verificar o sinal, exclui o caracter '-', deixando apenas o número*/
        for (int j = 0; j<tam-1; j++){
            numero[j] = numero[j+1];
        }
        tam--;

    } else{
        n->sinal = 1;
    } 

    int valor; // variável que vai guardar o valor de cada bloco do grande número
    int i, j; // variáveis auxiliares

    /*O seguinte loop permite a criação dos blocos*/
    while (tam > 0){
        valor = j = 0;

        /*O i indica a posição apartir da qual deve ser lida a string*/
        //De i até o fim da string, os char serão convertidos em int
        //o resultado será salvo em 'valor', que será usado para compor um novo nó
        if (tam >= qtdDigitos)
            i = tam - qtdDigitos;
        else
            i = 0;
        
        /*convertendo os digitos de char para int*/
        while (i < tam){
            valor = valor * 10 + (numero[i] - '0');
            j++; //j armazena a quantidade de digitos do valor 
            i++;
        }
        inserirBloco(n, valor);           

        tam = tam - j;
    }
    
    return n;
}

void inserirBloco(BigInt *n, int valor){
    if (!n) return; //caso o BigInt seja inválido

    bloco *b = malloc(sizeof(bloco));
    if (!b) return;

    /*==configurando bloco==*/
    b->num = valor;
    b->next = NULL;

    if (n->size == 0){
        //caso seja o primeiro nó
        n->tail = b;

    } else{
        bloco *tmp = n->tail;

        //percorre o BigInt até o último bloco
        for (int i = 1; i < n->size; i++){
            tmp = tmp->next;
        }

        tmp->next = b;
        b->previous = tmp;
    }
    n->head = b; //o bloco mais recente é sempre a cabeça, o bloco mais a esquerda
    n->size++;

}

/*===outras funções===*/

void imprimeBigInt(BigInt *n){
    if (!n) return; //verificando se ponteiro é válido

    //caso o grande número seja negativo, imprime sinal de negativo 
    if( n->sinal == -1) printf("-");

    /*começa a imprimir pelo último nó (bloco mais significativo)*/
    bloco *tmp = n->head;
    for (int i = 0; i < n->size; i++){
        printf("%d", tmp->num);
        tmp = tmp->previous;
    }
    
    printf("\n");
}