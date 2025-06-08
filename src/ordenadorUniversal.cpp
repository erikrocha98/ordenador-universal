#include <iostream>
#include "ordenadorUniversal.hpp"
#include "algOrdena.hpp"
#include "util.hpp"
#include <limits>
#include <cmath>
#include <iomanip>

void ordenadorUniversal(vetor *v, int tam, int minTamParticao, int limiarQuebras, sortperf_t *s)
{
    int r = tam - 1;
    int l = 0;
    int numeroQuebras = calculaQuebras(*v, tam);

    if (numeroQuebras < limiarQuebras)
    {
        insertionSort(v, l, r, s);
    }
    else
    {
        quickSort3Ins(v, l, r, minTamParticao, s);
    }
}

// Função para encontrar o índice do menor custo no array de estatísticas
int menorCusto(Estatisticas *estatisticas, int numEstatisticas)
{
    int indiceMinimo = 0;
    double custoMinimo = estatisticas[0].custo;

    for (int i = 1; i < numEstatisticas; i++)
    {
        if (estatisticas[i].custo < custoMinimo)
        {
            custoMinimo = estatisticas[i].custo;
            indiceMinimo = i;
        }
    }

    return indiceMinimo;
}

// Implementação da função para calcular a nova faixa para o limParticao
void calculaNovaFaixa(int limParticao, int *minMPS, int *maxMPS, int *passoMPS, Estatisticas *estatisticas, int numMPS)
{
    int newMin, newMax;

    if (limParticao == 0)
    {
        newMin = 0;
        newMax = 2;
    }
    else if (limParticao == numMPS - 1)
    {
        newMin = numMPS - 3;
        newMax = numMPS - 1;
    }
    else
    {
        newMin = limParticao - 1;
        newMax = limParticao + 1;
    }

    // Obtém os valores de MPS correspondentes aos índices
    *minMPS = estatisticas[newMin].parametro;
    *maxMPS = estatisticas[newMax].parametro;

    *passoMPS = (*maxMPS - *minMPS) / 5;
    if (*passoMPS == 0)
        (*passoMPS)++;
}

//Funções para encontrar o mínimo e o máximo entre dois números
int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Implementação da função para calcular a nova faixa para o limParticao
void calculaNovaFaixa2(int limQuebras, int *minLQ, int *maxLQ, int *passoLQ, Estatisticas *estatisticas, int numLQ)
{
    // Find index where limQuebras matches
    int limIndex = -1;
    for (int i = 0; i < numLQ; i++) {
        if (estatisticas[i*2].parametro == limQuebras) {
            limIndex = i;
            break;
        }
    }

    if (limIndex == 0) {
        *minLQ = limQuebras;
        *maxLQ = estatisticas[min(numLQ-1, 2)*2].parametro;
    }
    else if (limIndex == numLQ-1) {
        *minLQ = estatisticas[max(0, numLQ-3)*2].parametro;
        *maxLQ = limQuebras;
    }
    else {
        *minLQ = estatisticas[(limIndex-1)*2].parametro;
        *maxLQ = estatisticas[(limIndex+1)*2].parametro;
    }

    *passoLQ = (*maxLQ - *minLQ) / 5;
    if (*passoLQ == 0)
        (*passoLQ)++;
}

// Implementação da função para determinar o limiar de partição conforme Figura 2
int determinaLimiarParticao(vetor *v, int tam, double limiarCusto, double a, double b, double c)
{
    int minMPS = 2;
    int maxMPS = tam;
    int passoMPS = (maxMPS - minMPS) / 5;
    float diffCusto = std::numeric_limits<float>::max();
    int numMPS = 0;
    int t = 0;
    int limParticao = 0;
    int iteracao = 0;

    // Aloca memória para estatísticas
    Estatisticas *estatisticas = new Estatisticas[100];
    // Aloca memória para vetor temporário
    vetor vetorTemp;
    vetorTemp.tamanho = tam;
    vetorTemp.item = nullptr; // Inicializa como nullptr

    try
    {
        vetorTemp.item = new TipoItem[tam];

        while ((diffCusto > limiarCusto) && (passoMPS > 5))
        {
            std::cout<<std::endl;
            std::cout << "iter " << iteracao << std::endl;
            numMPS = 0;
            iteracao++;

            for (t = minMPS; t <= maxMPS; t += passoMPS)
            {
                sortperf_t s;
                resetcounter(&s);
                geraCopiaVetor(*v, &vetorTemp, tam);
                ordenadorUniversal(&vetorTemp, tam, t, 0, &s);

                double custoAtual = a * s.cmp + b * s.move + c * s.calls;

                estatisticas[numMPS].parametro = t;
                estatisticas[numMPS].comparacoes = s.cmp;
                estatisticas[numMPS].movimentacoes = s.move;
                estatisticas[numMPS].chamadas = s.calls;
                estatisticas[numMPS].custo = custoAtual;

                std::cout << "mps " << t << " cost " << std::fixed << std::setprecision(9) << custoAtual
                          << " cmp " << s.cmp << " move " << s.move
                          << " calls " << s.calls << std::endl;

                numMPS++;
            }

            limParticao = menorCusto(estatisticas, numMPS);
            calculaNovaFaixa(limParticao, &minMPS, &maxMPS, &passoMPS, estatisticas, numMPS);

            int maxMPSIndex = -1;
            int minMPSIndex = -1;
            for (int i = 0; i < numMPS; i++)
            {
                if (estatisticas[i].parametro == maxMPS)
                {
                    maxMPSIndex = i;
                    break;
                }
            }
            for (int i = 0; i < numMPS; i++)
            {
                if (estatisticas[i].parametro == minMPS)
                {
                    minMPSIndex = i;
                    break;
                }
            }

            diffCusto = fabs(estatisticas[minMPSIndex].custo - estatisticas[maxMPSIndex].custo);

            std::cout << "nummps " << numMPS << " limParticao " << estatisticas[limParticao].parametro
                      << " mpsdiff " << std::fixed << std::setprecision(6) << diffCusto << std::endl;
        }
    }
    catch (...)
    {
        // Libera memória em caso de exceção
        delete[] vetorTemp.item;
        delete[] estatisticas;
        throw; // Re-lança a exceção
    }

    int resultado = estatisticas[menorCusto(estatisticas, numMPS)].parametro;

    // Libera memória normalmente
    delete[] vetorTemp.item;
    delete[] estatisticas;

    return resultado;
}


int determinaLimiarQuebras(vetor *v, int tam, double limiarCusto, double a, double b, double c, int minParticao, int seed)
{
    // Inicializa ponteiros como nullptr para segurança
    
    Estatisticas *estatisticas = nullptr;
    vetor vTempOrdenado;
    vTempOrdenado.item = nullptr;
    vetor vTempQuickSort;
    vTempQuickSort.item = nullptr;
    vetor vTempInsercao;
    vTempInsercao.item = nullptr;

    try
    {
        // Alocações de memória
        estatisticas = new Estatisticas[200];
        vTempOrdenado.item = new TipoItem[tam];
        vTempQuickSort.item = new TipoItem[tam];
        vTempInsercao.item = new TipoItem[tam];
        vTempOrdenado.tamanho = vTempQuickSort.tamanho = vTempInsercao.tamanho = tam;

        // Inicializa vetor ordenado
        sortperf_t s;
        resetcounter(&s);
        geraVetorOrdenado(v, tam, minParticao, &s);


        int minLQ = 1;
        int maxLQ = tam / 2;
        int passoLQ = (maxLQ - minLQ) / 5;
        float diffCusto = std::numeric_limits<float>::max();
        int numLQ = 0, t = 0, limQuebras = 0, iteracao = 0;

        do 
        {
            std::cout << std::endl;
            std::cout << "iter " << iteracao++ << std::endl;
            numLQ = 0;

            for (t = minLQ; t <= maxLQ; t += passoLQ)
            {
                
                // Teste com QuickSort
                sortperf_t sQuick;
                resetcounter(&sQuick);
                srand48(seed);
                geraCopiaVetor(*v, &vTempQuickSort,tam);
                embaralhaVetor(&vTempQuickSort, tam, t, seed);
                /* ordenadorUniversal(&vTempQuickSort,tam,minParticao,0,&sQuick); */
                quickSort3Ins(&vTempQuickSort, 0, tam - 1, minParticao, &sQuick);

                estatisticas[numLQ * 2].parametro = t;
                estatisticas[numLQ * 2].custo = a * sQuick.cmp + b * sQuick.move + c * sQuick.calls;
                estatisticas[numLQ * 2].chamadas = sQuick.calls;
                estatisticas[numLQ * 2].comparacoes = sQuick.cmp;
                estatisticas[numLQ * 2].movimentacoes = sQuick.move;

                std::cout << "qs lq " << t << std::fixed << std::setprecision(9) << " cost " << estatisticas[numLQ * 2].custo
                          << " cmp " << sQuick.cmp << " move " << sQuick.move
                          << " calls " << sQuick.calls << std::endl;

                // Teste com Insertion Sort
                sortperf_t sInsert;
                resetcounter(&sInsert);
                srand48(seed);
                geraCopiaVetor(*v, &vTempInsercao, tam);
                embaralhaVetor(&vTempInsercao, tam, t, seed);
                insertionSort(&vTempInsercao, 0, tam - 1, &sInsert);

                estatisticas[numLQ * 2 + 1].parametro = t;
                estatisticas[numLQ * 2 + 1].custo = a * sInsert.cmp + b * sInsert.move + c * sInsert.calls;
                estatisticas[numLQ * 2 + 1].chamadas = sInsert.calls;
                estatisticas[numLQ * 2 + 1].comparacoes = sInsert.cmp;
                estatisticas[numLQ * 2 + 1].movimentacoes = sInsert.move;

                std::cout << "in lq " << t << " cost " << estatisticas[numLQ * 2 + 1].custo
                          << " cmp " << sInsert.cmp << " move " << sInsert.move
                          << " calls " << sInsert.calls << std::endl;

                numLQ++;
            }

            // Determina o limiar de quebras
            limQuebras = estatisticas[0].parametro;
            double menorDiff = fabs(estatisticas[1].custo - estatisticas[0].custo);

            for (int i = 1; i < numLQ; i++)
            {
                int insertIdx = i * 2 + 1;
                int quickIdx = i * 2;
                double diffAtual = fabs(estatisticas[insertIdx].custo - estatisticas[quickIdx].custo);

                if (diffAtual < menorDiff)
                {
                    menorDiff = diffAtual;
                    limQuebras = estatisticas[insertIdx].parametro;
                }

            }

            // Atualiza faixa de busca
            calculaNovaFaixa2(limQuebras, &minLQ, &maxLQ, &passoLQ, estatisticas, numLQ);

            // Cálculo da diferença de custo
            // Cálculo da diferença de custo
            int maxLQIndex = -1, minLQIndex = -1;
            for (int i = 0; i < numLQ; i++)
            {
                int insertIdx = i * 2 + 1;

                if (estatisticas[insertIdx].parametro == maxLQ)
                {
                    maxLQIndex = insertIdx; // Use insertion index for consistency
                }
                if (estatisticas[insertIdx].parametro == minLQ)
                {
                    minLQIndex = insertIdx; // Use insertion index for consistency
                }
            }

            // checagem de erro
            if (minLQIndex == -1 || maxLQIndex == -1)
            {
                std::cout << "Warning: Could not find min/max LQ indices" << std::endl;
                std::cout << "minLQ=" << minLQ << " maxLQ=" << maxLQ << std::endl;
                for (int i = 0; i < numLQ; i++)
                {
                    std::cout << "params[" << i << "]: quick="
                              << estatisticas[i * 2].parametro
                              << " insert=" << estatisticas[i * 2 + 1].parametro << std::endl;
                }
            }

            diffCusto = fabs(estatisticas[maxLQIndex].custo - estatisticas[minLQIndex].custo);

            std::cout << "numlq " << numLQ << " limQuebras " << limQuebras
                      << " lqdiff " << std::fixed << std::setprecision(6) << diffCusto << std::endl;
        }while ((diffCusto > limiarCusto) && (numLQ>=6));

        // Liberação de memória no fluxo normal
        delete[] vTempOrdenado.item;
        delete[] vTempQuickSort.item;
        delete[] vTempInsercao.item;
        delete[] estatisticas;

        return limQuebras;
    }
    catch (...)
    {
        // Liberação de memória em caso de exceção
        if (vTempOrdenado.item)
            delete[] vTempOrdenado.item;
        if (vTempQuickSort.item)
            delete[] vTempQuickSort.item;
        if (vTempInsercao.item)
            delete[] vTempInsercao.item;
        if (estatisticas)
            delete[] estatisticas;

        throw; // Re-lança a exceção após limpeza
    }
}