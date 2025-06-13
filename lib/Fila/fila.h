#pragma once
#include <Arduino.h>

typedef struct no {
    int numero;
    unsigned long tempo;
    struct no *proximo;
    struct no *anterior;
} No;

class fila {
public:
    fila();
    ~fila();

    void push(int valor);
    int pop();
    int media() const;
    unsigned long tempo_comeco_fila();

private:
    int qtd_;
    No *inicio_;
    No *fim_;
    int media_;
};