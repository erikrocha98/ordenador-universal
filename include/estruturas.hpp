#ifndef ESTRUTURAS_HPP
#define ESTRUTURAS_HPP
#include <iostream>

// Estrutura para contagem de operações
typedef struct sortperf {
    int cmp;    // número de comparações
    int move;   // número de movimentações
    int calls;  // número de chamadas
} sortperf_t;

// Estrutura para armazenar estatísticas
typedef struct Estatisticas {
    double custo;
    int comparacoes;
    int movimentacoes;
    int chamadas;
    int parametro;
} Estatisticas;


#endif