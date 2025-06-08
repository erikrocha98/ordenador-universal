#include <iostream>
#include "vetor.hpp"

TipoItem::TipoItem()
{
    chave = -1; // indica um item vazio
}
TipoItem::TipoItem(TipoChave c)
{
    chave = c;
}
void TipoItem::SetChave(TipoChave c)
{
    chave = c;
}
TipoChave TipoItem::GetChave()
{
    return chave;
}
void TipoItem::Imprime()
{
    std::cout<<chave<<" ";
}

vetor inicializaVetor(int tamanho) {
    vetor v;
    v.tamanho = tamanho;
    v.item = new TipoItem[tamanho];
    return v;
}

void destroiVetor(vetor* v) {
    delete[] v->item;
    v->item = nullptr;
    v->tamanho = 0;
}

void imprime (const vetor& v){
    for (int i=0; i<v.tamanho; i++){
        std::cout<<v.item[i].GetChave()<<" "<<std::endl;
    }
}