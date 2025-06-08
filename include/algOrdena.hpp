#ifndef SORTPERF_HPP
#define SORTPERF_HPP
#include "estruturas.hpp"
#include "vetor.hpp"

/* // Estrutura para armazenar métricas de desempenho
typedef struct sortperf {
    int cmp;    // Número de comparações
    int move;   // Número de movimentações
    int calls;  // Número de chamadas de função
} sortperf_t; */

void resetcounter(sortperf_t *s);
void inccmp(sortperf_t *s, int num);
void incmove(sortperf_t *s, int num);
void inccalls(sortperf_t *s, int num);
void swap(int *xp, int *yp, sortperf_t *s);
void insertionSort(vetor* v, int l, int r, sortperf_t *s);
int median(int a, int b, int c);
void partition3(vetor* v, int l, int r, int *i, int *j, sortperf_t *s);
void quickSort3Ins(vetor* v, int l, int r, int minTamParticao, sortperf_t *s);
void quickSort3(vetor *v, int l, int r, sortperf_t *s);


#endif // SORTPERF_HPP

