#include "sensores.h"
#include "fila.h"
#include <Arduino.h>

const float ADC_MAX_VALUE = 1023.0f;

Sensor::Sensor(int pino_adc, float referencia_volts, int tam_janela_media_movel, uint8_t qtd_leituras_para_media, float coef_a, float coef_b)
    : pino_adc_(pino_adc),
      tensao_referencia_(referencia_volts),
      tam_janela_media_movel_(tam_janela_media_movel),
      qtd_leituras_para_media_(qtd_leituras_para_media),
      coef_a_(coef_a),
      coef_b_(coef_b),
      media_movel_(),
      ultima_tensao_lida_(0.0f)
{
    pinMode(pino_adc_, INPUT);
    configurar_referencia_analogica_privada();
}

Sensor::~Sensor()
{
}

void Sensor::configurar_referencia_analogica_privada()
{
    if (abs(tensao_referencia_ - 1.1f) < 0.01f)
    {
        analogReference(INTERNAL);
    }
    else if (abs(tensao_referencia_ - 2.56f) < 0.01f)
    {
        analogReference(DEFAULT);
    }
    else
    {
        analogReference(DEFAULT);
    }
}

int Sensor::ler_valor_cru_adc()
{
    return analogRead(pino_adc_);
}

float Sensor::ler_tensao_instantanea()
{
    int valor_cru = ler_valor_cru_adc();
    ultima_tensao_lida_ = (static_cast<float>(valor_cru) / ADC_MAX_VALUE) * tensao_referencia_;
    return ultima_tensao_lida_;
}

float Sensor::ler_tensao_media()
{
    unsigned long tempo_primeiro_elemento = 0;
    if (media_movel_.qtd_ > 0) {
        tempo_primeiro_elemento = media_movel_.tempo_comeco_fila();
    }


    if (media_movel_.qtd_ == 0 || (millis() - tempo_primeiro_elemento > 1000 && media_movel_.qtd_ > 0) ) {
        for (uint8_t i = 0; i < qtd_leituras_para_media_; ++i) {
            if (media_movel_.qtd_ >= tam_janela_media_movel_ && tam_janela_media_movel_ > 0) {
                media_movel_.pop();
            }
            if (media_movel_.qtd_ < tam_janela_media_movel_ || tam_janela_media_movel_ == 0) {
                 media_movel_.push(ler_valor_cru_adc());
            } else if (tam_janela_media_movel_ > 0) {
                 media_movel_.push(ler_valor_cru_adc());
            }
        }
    } else {
        if (media_movel_.qtd_ > 0) {
             media_movel_.pop();
        }
        if (media_movel_.qtd_ < tam_janela_media_movel_ || tam_janela_media_movel_ == 0) {
            media_movel_.push(ler_valor_cru_adc());
        } else if (tam_janela_media_movel_ > 0) {
            media_movel_.push(ler_valor_cru_adc());
        }
    }

    if (media_movel_.qtd_ > 0) {
        float media_adc_cru = media_movel_.media();
        ultima_tensao_lida_ = (media_adc_cru / ADC_MAX_VALUE) * tensao_referencia_;
    } else {
    }
    return ultima_tensao_lida_;
}

float Sensor::calcular_grandeza()
{
    return (coef_a_ * ler_tensao_media()) + coef_b_;
}

float Sensor::get_ultima_tensao_calculada() const
{
    return ultima_tensao_lida_;
}