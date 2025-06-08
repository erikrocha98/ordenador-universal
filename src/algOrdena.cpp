#include <iostream>
#include "algOrdena.hpp"


/* typedef struct sortperf
{
  int cmp;
  int move;
  int calls;
} sortperf_t; */

void resetcounter(sortperf_t *s)
{
  // Descricao: inicializa estrutura
  // Entrada:
  // Saida: s
  s->cmp = 0;
  s->move = 0;
  s->calls = 0;
}

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

void swap(int *xp, int *yp, sortperf_t *s)
{
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
  incmove(s, 3);
}

// insertion sort
void insertionSort(vetor* v, int l, int r, sortperf_t *s)
{
  inccalls(s, 1);
  int aux, j = 0;
  for (int i = l+1; i < r+1; i++)
  {
    incmove(s,1);
    aux = v->item[i].GetChave();
    j = i - 1;

    inccmp(s, 1);
    while ((j >= 0) && (aux < v->item[j].GetChave()))
    {
      incmove(s, 1);
      v->item[j + 1].SetChave(v->item[j].GetChave());
      j--;
      inccmp(s, 1);
    }

    v->item[j + 1].SetChave(aux);
    incmove(s, 1);
  }
  return;
}

//Cálculo da mediana de 3
int median(int a, int b, int c)
{
  if ((a <= b) && (b <= c))
    return b; // a b c
  if ((a <= c) && (c <= b))
    return c; // a c b
  if ((b <= a) && (a <= c))
    return a; // b a c
  if ((b <= c) && (c <= a))
    return c; // b c a
  if ((c <= a) && (a <= b))
    return a; // c a b
  return b;   // c b a
}

// quicksort partition using median of 3
void partition3(vetor* v, int l, int r, int *i, int *j, sortperf_t *s)
{
  inccalls(s,1);
  int x;
  int centro_vetor = (l + r) / 2;
  *i = l;
  *j = r;
  x = median(v->item[l].GetChave(), v->item[centro_vetor].GetChave(), v->item[r].GetChave());

  do
  {
    while (x > v->item[*i].GetChave())
    {
      inccmp(s, 1);
      (*i)++;
    }
    inccmp(s, 1);
    while (x < v->item[*j].GetChave())
    {
      inccmp(s, 1);
      (*j)--;
    }
    inccmp(s, 1);

    if (*i <= *j)
    {
      int temp = v->item[*i].GetChave();
      v->item[*i].SetChave(v->item[*j].GetChave());
      v->item[*j].SetChave(temp);
      incmove(s, 3);
      (*i)++;
      (*j)--;
    }
  } while (*i <= *j);
}

// quicksort with insertion for small partitions and median of 3
void quickSort3Ins(vetor* v, int l, int r, int minTamParticao, sortperf_t *s)
{
  inccalls(s,1);
  int i, j;
  partition3(v, l, r, &i, &j, s);
  if(l<j){
    if (j - l< minTamParticao)
    {
      insertionSort(v, l, j, s);

    }
    else{
      quickSort3Ins(v, l, j, minTamParticao,s);
    }

  }
  if(i<r){
    if (r - i< minTamParticao) 
    {
      insertionSort(v, i, r, s);

    }
    else{
      quickSort3Ins(v, i, r, minTamParticao, s);
    }

  }
}

// quicksort with median of 3
void quickSort3(vetor *v, int l, int r, sortperf_t *s)
{
    inccalls(s,1);
  int i, j;
  partition3(v, l, r, &i, &j, s);

  // ordena o subvetor à esquerda do pivô
  if (l < j)
  {
    quickSort3(v, l, j, s);
  }
  // ordena o subvetor à direita do pivô
  if (i < r)
  {
    quickSort3(v, i, r, s);
  }
}



