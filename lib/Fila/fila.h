#pragma once
#include <Arduino.h>

/**
 * @brief Estrutura de um nó para a lista duplamente encadeada usada pela fila.
 */
typedef struct no {
    int numero;             ///< Valor inteiro armazenado no nó.
    unsigned long tempo;    ///< Timestamp (millis()) de quando o nó foi adicionado.
    struct no *proximo;     ///< Ponteiro para o próximo nó na fila.
    struct no *anterior;    ///< Ponteiro para o nó anterior na fila.
} No;

/**
 * @brief Classe que implementa uma fila (FIFO - First-In, First-Out)
 *        utilizando uma lista duplamente encadeada.
 *        Também calcula e mantém a média dos elementos na fila.
 */
class fila {
public:
    /**
     * @brief Construtor da classe fila. Inicializa uma fila vazia.
     */
    fila();

    /**
     * @brief Destrutor da classe fila. Libera a memória de todos os nós.
     */
    ~fila();

    /**
     * @brief Adiciona um novo valor ao final da fila.
     * @param valor O valor inteiro a ser adicionado.
     */
    void push(int valor);

    /**
     * @brief Remove e retorna o valor do início da fila.
     * @return O valor do início da fila. Retorna -1 (ou valor de erro definido na implementação) se a fila estiver vazia.
     */
    int pop();

    /**
     * @brief Retorna a média atual dos valores armazenados na fila.
     * @return A média dos valores como um inteiro.
     */
    int media() const;

    /**
     * @brief Retorna o timestamp de quando o primeiro elemento (início) da fila foi adicionado.
     * @return O timestamp (millis()) do elemento no início da fila.
     *         Pode causar comportamento indefinido se a fila estiver vazia e não for verificado antes.
     */
    unsigned long tempo_comeco_fila();

    /**
     * @brief Quantidade atual de elementos na fila.
     *        Acesso público para conveniência, mas idealmente seria acessado via um método getter.
     */
    int qtd_;

private:
    No *inicio_;    ///< Ponteiro para o primeiro nó da fila.
    No *fim_;       ///< Ponteiro para o último nó da fila.
    int media_;     ///< Média acumulada dos valores na fila.

};