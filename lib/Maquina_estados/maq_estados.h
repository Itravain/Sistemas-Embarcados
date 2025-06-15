#ifndef MAQ_ESTADOS_H
#define MAQ_ESTADOS_H

//Para usar utilize da seguinte forma
//Defina os estados antes no seu código
//Depois crie o objeto
//Exemplo:
//  enum Estados{inicio, meio, fim};
//  Maq_Estados<Estados> maq_estados(inicio);


#include <Arduino.h> // Para millis()

/**
 * @brief Classe template para gerenciamento de uma máquina de estados simples.
 * Permite definir estados, transitar entre eles e utilizar temporizadores não bloqueantes
 * e controle de primeira execução em um estado.
 * @tparam StateIdentifierType O tipo usado para identificar os estados (geralmente um enum).
 */
template <typename StateIdentifierType>
class Maq_Estados {
  private:
    bool _esta_contando_tempo;
    unsigned long _tempo_inicial_contador;
    bool _controle_primeira_vez;

  public:
    /**
     * @brief Identificador do estado atual da máquina.
     */
    StateIdentifierType estado_atual;

    /**
     * @brief Construtor da máquina de estados.
     * @param estado_de_partida O estado inicial da máquina.
     */
    Maq_Estados(StateIdentifierType estado_de_partida);

    /**
     * @brief Altera o estado atual da máquina para o próximo estado especificado.
     * Reseta o controle de 'primeira_vez_no_estado_atual' e o temporizador.
     * @param proximo_estado O estado para o qual a máquina deve transitar.
     */
    void mudar_estado(StateIdentifierType proximo_estado);

    /**
     * @brief Implementa um temporizador não bloqueante.
     * Na primeira chamada para uma nova contagem, registra o tempo inicial e retorna true (indicando que a contagem começou e o tempo não passou).
     * Nas chamadas subsequentes, verifica se a 'duracao_ms' já passou desde o início da contagem.
     * @param duracao_ms A duração do temporizador em milissegundos.
     * @return Retorna true se o tempo ainda não expirou (ou se a contagem acabou de iniciar).
     *         Retorna false se o tempo especificado já expirou.
     *         O contador é resetado (para uma nova contagem) quando o tempo expira ou quando `mudar_estado` é chamado.
     */
    bool contador_tempo(unsigned long duracao_ms);

    /**
     * @brief Verifica se é a primeira vez que o código dentro do estado atual está sendo executado
     * desde a última transição para este estado (via `mudar_estado`) ou desde a inicialização.
     * Útil para executar ações de inicialização de estado apenas uma vez.
     * @return Retorna true na primeira chamada após uma mudança de estado (ou na inicialização),
     *         e false nas chamadas subsequentes dentro do mesmo estado.
     *         É resetado para true quando `mudar_estado` é chamado.
     */
    bool primeira_vez_no_estado_atual();
};

template <typename StateIdentifierType>
Maq_Estados<StateIdentifierType>::Maq_Estados(StateIdentifierType estado_de_partida) {
    estado_atual = estado_de_partida;
    _controle_primeira_vez = false; // Será true na primeira chamada de primeira_vez_no_estado_atual()
    _esta_contando_tempo = false;
    _tempo_inicial_contador = 0;
}

template <typename StateIdentifierType>
void Maq_Estados<StateIdentifierType>::mudar_estado(StateIdentifierType proximo_estado) {
    estado_atual = proximo_estado;
    _controle_primeira_vez = false; // Reseta para a próxima chamada de primeira_vez_no_estado_atual()
    _esta_contando_tempo = false; // Reseta o temporizador ao mudar de estado
}

template <typename StateIdentifierType>
bool Maq_Estados<StateIdentifierType>::contador_tempo(unsigned long duracao_ms) {
    if (!_esta_contando_tempo) {
        _esta_contando_tempo = true;
        _tempo_inicial_contador = millis();
        return true; // Iniciou a contagem, tempo ainda não passou
    } else {
        if (millis() - _tempo_inicial_contador >= duracao_ms) {
            _esta_contando_tempo = false; // Tempo expirou, pronto para nova contagem
            return false; // Indica que o tempo passou
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