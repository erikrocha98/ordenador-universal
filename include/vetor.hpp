#ifndef VETOR_HPP
#define VETOR_HPP
#include <iostream>

typedef int TipoChave; // TipoChave é um inteiro

class TipoItem
{
public:
    TipoItem();
    TipoItem(TipoChave c);
    void SetChave(TipoChave c);
    TipoChave GetChave();
    void Imprime();

private:
    TipoChave chave;
    // outros membros
};

typedef struct vetor
{
    int tamanho;
    TipoItem *item;

} vetor;

// Métodos úteis para o vetor
vetor inicializaVetor(int tam);
void destroiVetor(vetor *v);
void imprime(const vetor& v);

#endif