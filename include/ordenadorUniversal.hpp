#ifndef ORDENADOR_UNIVERSAL_HPP
#define ORDENADOR_UNIVERSAL_HPP

#include "vetor.hpp"
#include "estruturas.hpp"

// Função principal do TAD Ordenador Universal
void ordenadorUniversal(vetor *v, int tam, int minTamParticao, int limiarQuebras, sortperf_t *s);

// Funções para determinar os limiares
int determinaLimiarParticao(vetor* v, int tam, double limiarCusto, double a, double b, double c);
int determinaLimiarQuebras(vetor* v, int tam, double limiarCusto, double a, double b, double c, int minParticao, int seed);

// Funções auxiliares
void calculaNovaFaixa(int limParticao, int* minMPS, int* maxMPS, int* passoMPS, Estatisticas* estatisticas, int numMPS);
int menorCusto(Estatisticas* estatisticas, int numEstatisticas);

#endif