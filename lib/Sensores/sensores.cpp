#include "Arduino.h"
#include "fila.h"

class Sensor{
private:
    int pino_;
    int media_atual_;
    short tensao_ref_;
    short ultima_tensao_lida;
    uint8_t qtd_media_movel_;
    fila media_movel_;

    int leitura();
public:
    Sensor(int pino_, short referencia, int tam_media_movel, uint8_t qtd_media_movel);
    ~Sensor();

    short tensao();
    float grandeza();
};

Sensor::Sensor(int pino, short referencia, int tam_media_movel, uint8_t qtd_media_movel)
{
    qtd_media_movel_ = qtd_media_movel;
    pino_ = pino;
    fila media_movel_;
    media_atual_ = 0;
    int leitura_atual_ = 0;
    tensao_ref_ = referencia;
    ultima_tensao_lida = 0;
    if (tensao_ref_ == 1.1)
    {
        analogReference(INTERNAL);
    }
}

Sensor::~Sensor()
{
}

int Sensor::leitura()
{
    ultima_tensao_lida = analogRead(pino_)*tensao_ref_/1023;
}

short Sensor:: tensao()
{
    if(millis() - media_movel_.tempo_comeco_fila() > 1000){    
        for (int i = 0; i < qtd_media_movel_; i++)
        {
            media_movel_.pop();
            media_movel_.push(leitura());
        }
    }
    else{
        media_movel_.pop();
        media_movel_.push(leitura());
    }
    ultima_tensao_lida = media_movel_.media();
    return ultima_tensao_lida;
}

float Sensor::grandeza(){

}