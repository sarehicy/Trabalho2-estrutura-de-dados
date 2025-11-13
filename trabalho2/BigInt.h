#ifndef BIGINT_H
#define BIGINT_H

typedef struct bigint BigInt;

//Cria struct para um grande número
//apartir de uma string contendo um número
BigInt *criarBigInt(char *numero);

//Cria "nó" do big int
//que contém o valor passado como parâmetro
void inserirBloco(BigInt *n, int valor);

//Imprime um grande número
void imprimeBigInt(BigInt *n);


void somarBig

#endif // BIGINT_H