#pragma once

#include <Arduino.h>
#include "fila.h" 

class Sensor {
public:
    /**
     * @brief Constructor for the Sensor class.
     * @param pino_adc The analog pin where the sensor is connected.
     * @param referencia_volts The ADC reference voltage (e.g., 5.0f for 5V, 3.3f for 3.3V, 1.1f for internal).
     * @param tam_janela_media_movel Intended size of the moving average window (for Fila).
     * @param qtd_leituras_para_media Number of readings to process in the moving average update logic.
     * @param coef_a Coefficient 'a' for linear conversion (grandeza = a * voltage + b).
     * @param coef_b Coefficient 'b' for linear conversion.
     */
    Sensor(int pino_adc,
           float referencia_volts = 5.0f,
           int tam_janela_media_movel = 10,
           uint8_t qtd_leituras_para_media = 1,
           float coef_a = 1.0f,
           float coef_b = 0.0f);

    /**
     * @brief Destructor for the Sensor class.
     */
    ~Sensor();

    // --- Primary Operational Methods ---

    /**
     * @brief Reads the raw ADC value directly from the sensor's pin.
     * @return The raw ADC value (e.g., 0-1023 for Arduino Uno, 0-4095 for ESP32).
     */
    int ler_valor_cru_adc();

    /**
     * @brief Reads the sensor, calculates the instantaneous voltage, and stores it internally.
     * This method updates the value retrieved by `get_ultima_tensao_calculada()`.
     * @return The instantaneous voltage as a float.
     */
    float ler_tensao_instantanea();

    /**
     * @brief Calculates and returns the voltage based on a moving average of raw ADC readings.
     * This method manages an internal Fila (`media_movel_`) to store raw ADC values.
     * The logic for updating the Fila is based on `qtd_leituras_para_media_` and timing, as seen in your .cpp.
     * This method also updates the value retrieved by `get_ultima_tensao_calculada()`.
     * @return The averaged voltage as a float.
     */
    float ler_tensao_media();

    /**
     * @brief Converts the last calculated voltage (stored internally) into a physical quantity.
     * Uses the linear transformation: physical_quantity = coef_a_ * voltage + coef_b_.
     * It is recommended to call `ler_tensao_instantanea()` or `ler_tensao_media()` before this
     * to ensure the internal voltage value is up-to-date.
     * @return The calculated physical quantity as a float.
     */
    float calcular_grandeza();

    // --- Getter ---
    /**
     * @brief Gets the last voltage value that was calculated and stored by either
     * `ler_tensao_instantanea()` or `ler_tensao_media()`.
     * @return The last stored voltage as a float.
     */
    float get_ultima_tensao_calculada() const;

private:
    // Configuration members
    int pino_adc_;
    float tensao_referencia_;
    int tam_janela_media_movel_; // Parameter from constructor, for Fila or other logic
    uint8_t qtd_leituras_para_media_;

    float coef_a_; // For y = ax + b conversion
    float coef_b_; // For y = ax + b conversion

    // State members
    fila media_movel_;          // IMPORTANT: This must be a member, not local to constructor.
    float ultima_tensao_lida_;  // Stores the result of the latest voltage calculation.

    // Private helper methods
    /**
     * @brief Configures the analog reference voltage for ADC readings if necessary.
     * Called internally by the constructor.
     */
    void configurar_referencia_analogica_privada();
};