#include <iostream>
#include <fstream>
#include <cstdlib>
#include "vetor.hpp"
#include "util.hpp"
#include "algOrdena.hpp"
#include "ordenadorUniversal.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_entrada>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Erro ao abrir arquivo: " << argv[1] << std::endl;
        return 1;
    }

    // Parâmetros de entrada
    int seed = 0;
    double limiarCusto = 0;
    double a, b, c = 0;
    int tam = 0;
    int minTamParticao = 0;
    
    // Leitura dos parâmetros do arquivo
    inputFile >> seed >> limiarCusto >> a >> b >> c >> tam;
    
    // Criação e preenchimento do vetor
    vetor v;
    v = inicializaVetor(tam);
    
    // Leitura dos valores do vetor do arquivo
    for (int i = 0; i < tam; i++) {
        int valor;
        inputFile >> valor;
        v.item[i].SetChave(valor);
    }
    
    inputFile.close();
    
    // Imprime o tamanho do vetor e a semente
    std::cout << "size " << tam << " seed " << seed << " breaks " << calculaQuebras(v, tam) << std::endl;
    
    // Parte 1: Determina o limiar de partição
    minTamParticao = determinaLimiarParticao(&v, tam, limiarCusto, a, b, c);
    
    // Parte 2: Determina o limiar de quebras
    determinaLimiarQuebras(&v, tam, limiarCusto, a, b, c, minTamParticao, seed);
    
    // Liberação de memória
    destroiVetor(&v);

    return 0;
}