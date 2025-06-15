#include "Arduino.h"
#include "fila.h"

fila::fila(){
    qtd_ = 0;
    inicio_ = NULL;
    fim_ = NULL;
    media_ = 0.0f; // Inicializar como float
}

fila::~fila(){
    No* atual = inicio_;
    while (atual != NULL) {
        No* proximo_no = atual->proximo;
        free(atual);
        atual = proximo_no;
    }
    inicio_ = NULL;
    fim_ = NULL;
    qtd_ = 0;   
}

void fila::push(int valor) {
    No *novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        return;
    }
    novo->numero = valor;
    novo->tempo = millis();
    novo->proximo = NULL; 
    novo->anterior = fim_; 

    if (fim_ != NULL) {
        fim_->proximo = novo;
    }
    fim_ = novo;

    if (inicio_ == NULL) {
        inicio_ = novo;
    }
    qtd_++;

    // Atualiza a média
    if (qtd_ == 1) {
        media_ = static_cast<float>(valor);
    } else {
        // Garantir aritmética de ponto flutuante
        media_ = (media_ * static_cast<float>(qtd_ - 1) + static_cast<float>(valor)) / static_cast<float>(qtd_);
    }
}

int fila::pop() {
    if (inicio_ == NULL) {
        return -1; 
    }
    No *auxiliar = inicio_;
    int numero = inicio_->numero;

    if (qtd_ == 1) {
        inicio_ = NULL;
        fim_ = NULL;
    } else {
        inicio_ = inicio_->proximo;
        inicio_->anterior = NULL;
    }
    free(auxiliar);
    qtd_--; // Decrementa qtd_ ANTES de recalcular a média

    if (qtd_ > 0) {
        // Garantir aritmética de ponto flutuante. Usar qtd_ (nova quantidade) no denominador.
        // A soma total anterior era media_ (antiga) * (qtd_ + 1) (quantidade antiga)
        media_ = (media_ * static_cast<float>(qtd_ + 1) - static_cast<float>(numero)) / static_cast<float>(qtd_);
    } else {
        media_ = 0.0f; // Fila vazia, média é 0
    }
    return numero;
}

float fila::media() const // Modificado de int para float
{
    return media_;
}

unsigned long fila::tempo_comeco_fila(){
    return inicio_->tempo;
}