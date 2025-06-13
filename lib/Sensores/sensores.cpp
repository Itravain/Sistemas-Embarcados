#include "Arduino.h"
#include "fila.h"

class Sensor{
private:
    int pino_;
    int media_atual_;
    fila media_movel_;
    int leitura();
public:
    Sensor(int pino_);
    ~Sensor();
};

Sensor::Sensor(int pino)
{
    pino_ = pino;
    fila media_movel_;
    int leitura_atual_ = 0;
}

Sensor::~Sensor()
{
}

int Sensor::leitura()
{
    if (media )
    {
        /* code */
    }
    
}