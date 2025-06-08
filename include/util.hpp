#ifndef UTIL_HPP
#define UTIL_HPP
#include <iostream>
#include "vetor.hpp"
#include "estruturas.hpp"

int calculaQuebras(const vetor& v, int tam);
void geraVetorOrdenado(vetor* v, int size, int minParticao, sortperf_t* s);
void geraVetorInversamenteOrdenado(vetor* v, int size);
void geraVetorComQuebras (vetor* v, int size, int numQuebras, int seed);
void geraCopiaVetor(const vetor& vetorOrigem, vetor* vetorDestino, int size);
void embaralhaVetor(vetor* v, int size, int numShuffle, int seed);
bool verificaOrdenacao(const vetor& v, int tam);
double calculaCusto(sortperf_t *s, double a, double b, double c);
void registraEstatisticas(sortperf_t *s, int indice, double a, double b, double c);
void imprimeEstatisticas(double custo, int numMPS, int comparacoes, int movimentacoes, int chamadas);

#endif