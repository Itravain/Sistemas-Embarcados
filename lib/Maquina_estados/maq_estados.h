#ifndef MAQ_ESTADOS_H
#define MAQ_ESTADOS_H

//Para usar utilize da seguinte forma
//Defina os estados antes no seu código
//Depois crie o objeto
//Exemplo:
//  enum Estados{inicio, meio, fim};
//  Maq_Estados<Estados> maq_estados(inicio);


#include <Arduino.h> // Para millis()


template <typename StateIdentifierType>
class Maq_Estados {
  private:
    bool _esta_contando_tempo;
    unsigned long _tempo_inicial_contador;
    bool _controle_primeira_vez;

  public:
    StateIdentifierType estado_atual; // O tipo do estado atual é o tipo do template

    // Construtor: recebe o estado inicial do tipo do template
    Maq_Estados(StateIdentifierType estado_de_partida);

    // Muda o estado atual da máquina
    void mudar_estado(StateIdentifierType proximo_estado);

    // Temporizador não bloqueante
    bool contador_tempo(unsigned long duracao_ms);

    // Verifica se é a primeira vez que o estado atual está sendo executado
    // desde a última chamada a mudar_estado() ou desde a inicialização.
    bool primeira_vez_no_estado_atual();
};

template <typename StateIdentifierType>
Maq_Estados<StateIdentifierType>::Maq_Estados(StateIdentifierType estado_de_partida) {
    estado_atual = estado_de_partida;
    _controle_primeira_vez = false;
    _esta_contando_tempo = false;
    _tempo_inicial_contador = 0;
}

template <typename StateIdentifierType>
void Maq_Estados<StateIdentifierType>::mudar_estado(StateIdentifierType proximo_estado) {
    estado_atual = proximo_estado;
    _controle_primeira_vez = false;
    _esta_contando_tempo = false;
}

template <typename StateIdentifierType>
bool Maq_Estados<StateIdentifierType>::contador_tempo(unsigned long duracao_ms) {
    if (!_esta_contando_tempo) {
        _esta_contando_tempo = true;
        _tempo_inicial_contador = millis();
        return true; // Iniciou a contagem, tempo ainda não passou
    } else {
        if (millis() - _tempo_inicial_contador >= duracao_ms) {
            _esta_contando_tempo = false; // Tempo expirou
            return false;
        } else {
            return true; // Tempo ainda não expirou
        }
    }
}

template <typename StateIdentifierType>
bool Maq_Estados<StateIdentifierType>::primeira_vez_no_estado_atual() {
    if (!_controle_primeira_vez) {
        _controle_primeira_vez = true;
        return true;
    } else {
        return false;
    }
}

#endif