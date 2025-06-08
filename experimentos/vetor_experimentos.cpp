// vetor_experimento.hpp - Estrutura conforme enunciado
#ifndef VETOR_EXPERIMENTO_HPP
#define VETOR_EXPERIMENTO_HPP

#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iostream>

// Estrutura exatamente como no enunciado
typedef struct Item {
    char key[KEYSZ];      // Chave como string de dígitos
    char payload[PLSZ];   // Payload
} TipoItem;

// Para manter compatibilidade com seu código
typedef struct {
    TipoItem* item;
    int tamanho;
} vetor;

// Funções auxiliares
inline void geraChaveString(char* key, int valor, int tamanho) {
    // Preenche com zeros à esquerda
    for (int i = tamanho - 2; i >= 0; i--) {
        key[i] = '0' + (valor % 10);
        valor /= 10;
    }
    key[tamanho - 1] = '\0';  // Null terminator
}

inline int comparaChaves(const char* key1, const char* key2) {
    return strcmp(key1, key2);  // Comparação de strings
}

// Adaptação para seu código
class ItemAdapter {
public:
    static void SetChave(TipoItem& item, int valor) {
        long mult = (long)pow(10, KEYSZ - 1);
        int val = valor % mult;  // Garante que cabe em KEYSZ dígitos
        geraChaveString(item.key, val, KEYSZ);
    }
    
    static int GetChave(const TipoItem& item) {
        // Converte string para int (para compatibilidade)
        return atoi(item.key);
    }
    
    static int Compara(const TipoItem& a, const TipoItem& b) {
        return strcmp(a.key, b.key);
    }
    
    static void GeraAleatorio(TipoItem& item) {
        long mult = (long)pow(10, KEYSZ - 1);
        int valor = (int)(drand48() * mult);
        geraChaveString(item.key, valor, KEYSZ);
        
        // Preenche payload
        for (int i = 0; i < PLSZ - 1; i++) {
            item.payload[i] = '0' + (i % 10);
        }
        item.payload[PLSZ - 1] = '\0';
    }
};

// Macros para os algoritmos
#define COMPARA_KEYS(a, b) strcmp((a).key, (b).key)
#define CHAVE_MENOR(a, b) (COMPARA_KEYS(a, b) < 0)
#define CHAVE_MAIOR(a, b) (COMPARA_KEYS(a, b) > 0)
#define CHAVE_IGUAL(a, b) (COMPARA_KEYS(a, b) == 0)

#endif