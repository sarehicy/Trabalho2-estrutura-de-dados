#ifndef BIGINT_H
#define BIGINT_H

typedef struct bigint BigInt;

//Cria struct para um grande número
//apartir de uma string contendo um número
BigInt *criarBigInt(char *numero);

//Cria "nó" do big int
//que contém o valor passado como parâmetro
void inserirBloco(BigInt *n, int valor);

//Desaloca todo espaço utilizado na criação do big int
void destruirBigInt(BigInt **n);

//Imprime um grande número
void imprimeBigInt(BigInt *n);

//Soma dois grandes números
BigInt *somarBigInt(BigInt *a, BigInt *b);

//Subtrai dois grandes números
//auxiliar da função somarBigInt()
void subtrairBigInt(BigInt *a, BigInt *b, int sizeSoma, int *i, int *somas);

//Compara dois grandes números
//Retorna 1 se a>b, 2 se b>a ou 3 se a = b
int comparaBigInt(BigInt *a, BigInt *b);



#endif // BIGINT_H