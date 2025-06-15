#pragma once

#include <Arduino.h>
#include "fila.h" 

class Sensor {
public:
    /**
     * @brief Construtor para a classe Sensor.
     * @param pino_adc O pino analógico onde o sensor está conectado.
     * @param referencia_volts A tensão de referência do ADC (ex: 5.0f para 5V, 3.3f para 3.3V, 1.1f para interna).
     * @param tam_janela_media_movel Tamanho desejado da janela de média móvel (para a Fila).
     * @param qtd_leituras_para_media Número de leituras a serem processadas na lógica de atualização da média móvel.
     * @param coef_a Coeficiente 'a' para conversão linear (grandeza = a * tensao + b).
     * @param coef_b Coeficiente 'b' para conversão linear.
     */
    Sensor(int pino_adc,
           float referencia_volts = 5.0f,
           int tam_janela_media_movel = 10,
           uint8_t qtd_leituras_para_media = 1,
           float coef_a = 1.0f,
           float coef_b = 0.0f);

    /**
     * @brief Destrutor para a classe Sensor.
     */
    ~Sensor();

    // --- Métodos Operacionais Primários ---

    /**
     * @brief Lê o valor ADC bruto diretamente do pino do sensor.
     * @return O valor ADC bruto (ex: 0-1023 para Arduino Uno, 0-4095 para ESP32).
     */
    int ler_valor_cru_adc();

    /**
     * @brief Lê o sensor, calcula a tensão instantânea e a armazena internamente.
     * Este método atualiza o valor recuperado por `get_ultima_tensao_calculada()`.
     * @return A tensão instantânea como um float.
     */
    float ler_tensao_instantanea();

    /**
     * @brief Calcula e retorna a tensão com base em uma média móvel de leituras ADC brutas.
     * Este método gerencia uma Fila interna (`media_movel_`) para armazenar valores ADC brutos.
     * A lógica para atualizar a Fila é baseada em `qtd_leituras_para_media_` e temporização, conforme visto no seu .cpp.
     * Este método também atualiza o valor recuperado por `get_ultima_tensao_calculada()`.
     * @return A tensão média como um float.
     */
    float ler_tensao_media();

    /**
     * @brief Converte a última tensão calculada (armazenada internamente) em uma grandeza física.
     * Usa a transformação linear: grandeza_fisica = coef_a_ * tensao + coef_b_.
     * Recomenda-se chamar `ler_tensao_instantanea()` ou `ler_tensao_media()` antes deste método
     * para garantir que o valor da tensão interna esteja atualizado.
     * @return A grandeza física calculada como um float.
     */
    float calcular_grandeza();

    // --- Getter ---
    /**
     * @brief Obtém o último valor de tensão que foi calculado e armazenado por
     * `ler_tensao_instantanea()` ou `ler_tensao_media()`.
     * @return A última tensão armazenada como um float.
     */
    float get_ultima_tensao_calculada() const;

private:
    // Membros de configuração
    int pino_adc_;
    float tensao_referencia_;
    int tam_janela_media_movel_; 
    uint8_t qtd_leituras_para_media_;

    float coef_a_; 
    float coef_b_; 

    // Membros de estado
    fila media_movel_;          
    float ultima_tensao_lida_;  

    // Métodos auxiliares privados
    /**
     * @brief Configura a tensão de referência analógica para leituras ADC, se necessário.
     * Chamado internamente pelo construtor.
     */
    void configurar_referencia_analogica_privada();
};