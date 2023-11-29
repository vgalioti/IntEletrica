#include "Grafico.h"
#include <iostream>
using namespace std;

#include <cstdlib>

void imprimir(double *seq, int tamanho) {
    for(int i = 0; i < tamanho; i++) {
        cout << seq[i] << "; ";
    }

    cout << endl;
}

void teste();

int main () {
    teste();
    return 0;
}

void teste() {
    /////// BASE ALEATORIA
    srand(2167);

    double *fonte = new double[100];

    for(int i = 0; i < 50; i++) {
        fonte[i] = (rand() % (55)) + 5;
    }

    for(int i = 50; i < 100; i++) {
        fonte[i] = 50;
    }

    imprimir(fonte, 100);
    cout << endl;

    Grafico *g1 = new Grafico("Teste", fonte, 100);
    g1->plot();

    double final[100];
    ////// FIM BASE ALEATORIA

    final[0] = fonte[0];
    double diferenca;

    for(int i = 1; i < 100; i++) {
        diferenca = (fonte[i] - final[i-1]) * 0.2;
        if(diferenca > 10) diferenca = 10;
        else if(diferenca < -10) diferenca = -10;

        final[i] = final[i-1] + diferenca;
    }

    imprimir(final, 100);
    Grafico *g2 = new Grafico("Teste", final, 100);
    g2->plot();
    
}
