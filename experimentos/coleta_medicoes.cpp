// coleta_medicoes_v2.cpp - Implementação conforme enunciado
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;
using namespace std::chrono;

// Estrutura EXATAMENTE como no enunciado
typedef struct item {
    char key[KEYSZ];
    char payload[PLSZ];
} item_t;

// Estrutura para estatísticas
typedef struct {
    long cmp;
    long move;
    long calls;
} sortperf_t;

void inccmp(sortperf_t *s, int num)
{
  // Descricao: incrementa o numero de comparacoes em num
  // Entrada: s, num
  // Saida: s
  s->cmp += num;
}

void incmove(sortperf_t *s, int num)
{
  // Descricao: incrementa o numero de movimentacoes de dados em num
  // Entrada: s, num
  // Saida: s
  s->move += num;
}

void inccalls(sortperf_t *s, int num)
{
  // Descricao: incrementa o numero de chamadas de função em num
  // Entrada: s, num
  // Saida: s
  s->calls += num;
}

// Funções auxiliares
void resetcounter(sortperf_t* s) {
    s->cmp = 0;
    s->move = 0;
    s->calls = 0;
}

// Comparação que conta
int compara(const item_t& a, const item_t& b, sortperf_t* s) {
    s->cmp++;
    return strcmp(a.key, b.key);
}

// Troca que conta
void troca(item_t& a, item_t& b, sortperf_t* s) {
    s->move++;
    item_t temp = a;
    a = b;
    b = temp;
}

// standard quicksort partition
void partition(item_t *A, int l, int r, int *i, int *j, sortperf_t *s)
{
    inccalls(s,1);
  item_t x, w;
  *i = l;
  *j = r;
  x = A[(*i + *j) / 2];
  do
  {
    while (strcmp(x.key, A[*i].key)>0)
    {
      inccmp(s, 1);
      (*i)++;
    }
    inccmp(s, 1);
    
    while (strcmp(x.key, A[*j].key)<0)
    {
      inccmp(s, 1);
      (*j)--;
    }
    inccmp(s, 1);
    
    if (*i <= *j)
    {
      incmove(s, 3);
      w = A[*i];
      A[*i] = A[*j];
      A[*j] = w;
      (*i)++;
      (*j)--;
    }
  } while (*i <= *j);

}

// standard quicksort
void quickSort(item_t *A, int l, int r, sortperf_t *s)
{
    inccalls(s,1);
  int i, j;
  partition(A, l, r, &i, &j, s);

  // ordena o subvetor à esquerda do pivô
  
  if (l < j)
  {
    quickSort(A, l, j, s);
  }
  // ordena o subvetor à direitra do pivô
  if (i < r)
  {
    quickSort(A, i, r, s);
  }
}

// Tipos de configuração do vetor
enum TipoVetor {
    ALEATORIO,
    ORDENADO,
    INVERSO,
    QUASE_ORDENADO_10,
    QUASE_ORDENADO_25,
    QUASE_ORDENADO_50
};

// Gera chave para um valor específico
void geraChave(char* key, int valor) {
    for (int p = KEYSZ - 2; p >= 0; p--) {
        key[p] = '0' + (valor % 10);
        valor /= 10;
    }
    key[KEYSZ - 1] = '\0';
}

// Gera vetor aleatório
void geraVetorAleatorio(item_t* vet, int tam) {
    long mult = (long)pow(10, KEYSZ - 1);
    
    for (int i = 0; i < tam; i++) {
        int valor = (int)(drand48() * mult);
        geraChave(vet[i].key, valor);
        
        // Preenche payload
        for (int j = 0; j < PLSZ - 1; j++) {
            vet[i].payload[j] = '0' + (j % 10);
        }
        vet[i].payload[PLSZ - 1] = '\0';
    }
}

// Gera vetor ordenado
/* void geraVetorOrdenado(item_t* vet, int tam) {
    long mult = (long)pow(10, KEYSZ - 1);
    
    for (int i = 0; i < tam; i++) {
        // Distribui valores uniformemente no intervalo
        int valor = (int)((double)i / tam * mult);
        geraChave(vet[i].key, valor);
        
        // Preenche payload
        for (int j = 0; j < PLSZ - 1; j++) {
            vet[i].payload[j] = '0' + (j % 10);
        }
        vet[i].payload[PLSZ - 1] = '\0';
    }
} */

// Gera vetor inversamente ordenado
/* void geraVetorInverso(item_t* vet, int tam) {
    long mult = (long)pow(10, KEYSZ - 1);
    
    for (int i = 0; i < tam; i++) {
        // Valores decrescentes
        int valor = (int)((double)(tam - i - 1) / tam * mult);
        geraChave(vet[i].key, valor);
        
        // Preenche payload
        for (int j = 0; j < PLSZ - 1; j++) {
            vet[i].payload[j] = '0' + (j % 10);
        }
        vet[i].payload[PLSZ - 1] = '\0';
    }
}
 */
// Gera vetor quase ordenado com percentual de quebras
/* void geraVetorQuaseOrdenado(item_t* vet, int tam, double percentualQuebras) {
    // Primeiro gera ordenado
    geraVetorOrdenado(vet, tam);
    
    // Calcula número de trocas necessárias
    int numTrocas = (int)(tam * percentualQuebras);
    
    // Realiza trocas aleatórias
    for (int i = 0; i < numTrocas; i++) {
        int pos1 = (int)(drand48() * tam);
        int pos2 = (int)(drand48() * tam);
        
        // Troca os elementos
        item_t temp = vet[pos1];
        vet[pos1] = vet[pos2];
        vet[pos2] = temp;
    }
}
 */
// Função genérica para gerar vetor baseado no tipo
void geraVetor(item_t* vet, int tam, TipoVetor tipo) {
    switch(tipo) {
        case ALEATORIO:
            geraVetorAleatorio(vet, tam);
            break;
        /* case ORDENADO:
            geraVetorOrdenado(vet, tam);
            break;
        case INVERSO:
            geraVetorInverso(vet, tam);
            break;
        case QUASE_ORDENADO_10:
            geraVetorQuaseOrdenado(vet, tam, 0.10);
            break;
        case QUASE_ORDENADO_25:
            geraVetorQuaseOrdenado(vet, tam, 0.25);
            break;
        case QUASE_ORDENADO_50:
            geraVetorQuaseOrdenado(vet, tam, 0.50);
            break; */
    }
}

// Nome do tipo para arquivo
const char* nomeTipoVetor(TipoVetor tipo) {
    switch(tipo) {
        case ALEATORIO: return "aleatorio";
        case ORDENADO: return "ordenado";
        case INVERSO: return "inverso";
        case QUASE_ORDENADO_10: return "quase_ord_10";
        case QUASE_ORDENADO_25: return "quase_ord_25";
        case QUASE_ORDENADO_50: return "quase_ord_50";
        default: return "desconhecido";
    }
}

void coletarMedicoes(int minTam, int maxTam, int numPontos, string arquivoSaida) {
    // Lista de tipos de vetores a testar
    TipoVetor tipos[] = {
        ALEATORIO
    };
    int numTipos = 1;
    
    cout << "=== COLETA COMPLETA (Estrutura do Enunciado) ===" << endl;
    cout << "KEYSZ: " << KEYSZ << " (string de " << KEYSZ-1 << " dígitos)" << endl;
    cout << "PLSZ: " << PLSZ << " bytes" << endl;
    cout << "Configuração: apenas vetor aleatório" << endl;
    cout << "Tamanhos: " << minTam << " a " << maxTam << endl << endl;
    
    int passo = (maxTam - minTam) / (numPontos - 1);
    if (passo < 1) passo = 1;
    
    // Para cada tipo de vetor
    for (int t = 0; t < numTipos; t++) {
        TipoVetor tipo = tipos[t];
        
        // Nome do arquivo de saída
        stringstream ss;
        ss << "medicoes_k" << KEYSZ << "_p" << PLSZ << "_" << nomeTipoVetor(tipo) << ".txt";
        string arquivoSaida = ss.str();
        
        ofstream saida(arquivoSaida);
        if (!saida.is_open()) {
            cerr << "Erro ao criar arquivo: " << arquivoSaida << endl;
            continue;
        }
        
        cout << "\n--- Tipo: " << nomeTipoVetor(tipo) << " ---" << endl;
        cout << "Arquivo: " << arquivoSaida << endl;
        
        saida << "# tamanho comparacoes movimentacoes chamadas tempo_microsegundos" << endl;
        saida << "# KEYSZ=" << KEYSZ << " PLSZ=" << PLSZ << " Tipo=" << nomeTipoVetor(tipo) << endl;
        
        // Para cada tamanho
        for (int i = 0; i < numPontos; i++) {
            int tam = minTam + i * passo;
            if (tam > maxTam) tam = maxTam;
            
            cout << "  Tamanho " << tam << "... " << flush;
            
            // Aloca vetores
            item_t* vet = new item_t[tam];
            item_t* vet_copia = new item_t[tam];
            
            // Gera vetor base do tipo específico
            geraVetor(vet, tam, tipo);
            
            const int numExec = 100;
            vector<long> tempos, cmps, moves, calls;
            
            for (int exec = 0; exec < numExec; exec++) {
                // Copia vetor
                memcpy(vet_copia, vet, tam * sizeof(item_t));
                
                sortperf_t s;
                resetcounter(&s);
                
                auto inicio = high_resolution_clock::now();
                
                quickSort(vet_copia, 0, tam - 1, &s);
                
                auto fim = high_resolution_clock::now();
                auto duracao = duration_cast<microseconds>(fim - inicio);
                
                tempos.push_back(duracao.count());
                cmps.push_back(s.cmp);
                moves.push_back(s.move);
                calls.push_back(s.calls);
            }
            
            delete[] vet;
            delete[] vet_copia;
            
            // Calcular médias
            long tempo_medio = 0, cmp_medio = 0, move_medio = 0, call_medio = 0;
            
            for (int j = 0; j < numExec; j++) {
                tempo_medio += tempos[j];
                cmp_medio += cmps[j];
                move_medio += moves[j];
                call_medio += calls[j];
            }
            
            tempo_medio /= numExec;
            cmp_medio /= numExec;
            move_medio /= numExec;
            call_medio /= numExec;
            
            saida << tam << " " 
                  << cmp_medio << " " 
                  << move_medio << " " 
                  << call_medio << " " 
                  << tempo_medio << endl;
            
            cout << "OK (tempo: " << tempo_medio << " μs, cmp: " << cmp_medio << ")" << endl;
        }
        
        saida.close();
    }
    
    cout << "\nColeta completa concluída!" << endl;
    cout << "Foram gerados " << numTipos << " arquivos de medições." << endl;
}

int main(int argc, char* argv[]) {
    int minTam = 1000;
    int maxTam = 20000;  // Menor porque strcmp é mais lento
    int numPontos = 40;
    
    stringstream ss;
    ss << "medicoes_v2_k" << KEYSZ << "_p" << PLSZ << ".txt";
    string arquivoSaida = ss.str();
    
    if (argc > 1) minTam = atoi(argv[1]);
    if (argc > 2) maxTam = atoi(argv[2]);
    if (argc > 3) numPontos = atoi(argv[3]);
    if (argc > 4) arquivoSaida = argv[4];
    
    coletarMedicoes(minTam, maxTam, numPontos, arquivoSaida);
    
    return 0;
}