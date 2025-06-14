#include "Arduino.h"
#include "fila.h"

fila::fila(){
    qtd_ = 0;
    inicio_ = NULL;
    fim_ = NULL;
    media_ = 0;
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
        media_ = valor;
    } else {
        media_ = (media_ * (qtd_ - 1) + valor) / qtd_;
    }
}

int fila::pop() {
    if (inicio_ == NULL) {
        return -1; // Ou alguma outra indicação de erro/fila vazia
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
    qtd_--;

    if (qtd_ > 0) {
        media_ = (media_ * (qtd_ + 1) - numero) / qtd_;
    } else {
        media_ = 0;
    }
    return numero;
}

int fila::media() const
{
    return media_;
}

unsigned long fila::tempo_comeco_fila(){
    return inicio_->tempo;
}