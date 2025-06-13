#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {
  private:
    bool aceso; 
    uint64_t _intensidade; 

  public:
    int pino; 
    unsigned long tempo_inicial; 

    Led(int p);
    void estado(int estado);
    void intensidade(float intensidade);
    bool acende_tempo(unsigned long tempo);
};

#endif