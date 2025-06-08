#include <iostream>
#include "util.hpp"
#include "algOrdena.hpp"
#include <cstdlib>

int calculaQuebras(const vetor &v, int tam)
{
    int quebras = 0;
    for (int i = 1; i < tam; i++)
    {
        if (v.item[i - 1].GetChave() > v.item[i].GetChave())
        {
            quebras++;
        }
    }
    return quebras;
}

void embaralhaVetor(vetor *v, int size, int numShuffle,int seed)
{
    srand48(seed);
    int p1 = 0, p2 = 0, temp;
    for (int t = 0; t < numShuffle; t++)
    {
        /* Gera dois índices distintos no intervalo [0..size-1] */
        while (p1 == p2)
        {
            p1 = (int)(drand48() * size);
            p2 = (int)(drand48() * size);
        }
        /* Realiza a troca para introduzir uma quebra */
        temp = v->item[p1].GetChave();
        v->item[p1].SetChave(v->item[p2].GetChave());
        v->item[p2].SetChave(temp);
        p1 = p2 = 0;
    }
}

void geraVetorOrdenado(vetor *v, int size, int minParticao, sortperf_t* s)
{
    int l=0;
    int r= size-1;
    quickSort3Ins(v, l, r,minParticao,s);
}


void geraVetorInversamenteOrdenado(vetor *v, int size)
{
    for (int i = 0; i < size; i++)
    {
        v->item[i].SetChave(size - 1 - i);
    }
}

// Geramos cópias dos vetores para podermos realizar os testes com a segurança de não estar modificando o vetor original.
void geraCopiaVetor(const vetor &vetorOrigem, vetor *vetorDestino, int size)
{
    for (int i = 0; i < size; i++)
    {
        vetorDestino->item[i].SetChave(vetorOrigem.item[i].GetChave());
    }
}

// Verifica se o vetor está ordenado
bool verificaOrdenacao(const vetor &v, int tam)
{
    for (int i = 1; i < tam; i++)
    {
        if (v.item[i - 1].GetChave() > v.item[i].GetChave())
        {
            return false;
        }
    }
    return true;
}

double calculaCusto(sortperf_t *s, double a, double b, double c)
{
    double result = a * s->cmp + b * s->move + c * s->calls;
    return result;
}

// Funções para as estatísticas

// Array global para armazenar estatísticas
Estatisticas estatisticas[100];
int numEstatisticas = 0;

void registraEstatisticas(sortperf_t *s, int indice, double a, double b, double c)
{
    estatisticas[indice].custo = calculaCusto(s, a, b, c);
    estatisticas[indice].chamadas = s->calls;
    estatisticas[indice].comparacoes = s->cmp;
    estatisticas[indice].movimentacoes = s->move;

    numEstatisticas = (indice >= numEstatisticas) ? indice + 1 : numEstatisticas;
}

void imprimeEstatisticas(double custo, int numMPS, int comparacoes, int movimentacoes, int chamadas)
{
    std::cout << "mps " << numMPS << " cost " << std::fixed << custo
              << " cmp " << comparacoes << " move " << movimentacoes
              << " calls " << chamadas << std::endl;
}
