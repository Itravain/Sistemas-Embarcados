#pragma once
#include <Arduino.h>

typedef struct no {
    int numero;
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

private:
    int qtd_;
    No *inicio_;
    No *fim_;
    int media_;
};