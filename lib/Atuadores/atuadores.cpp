#include <Arduino.h>

class Atuador{
protected:
    uint8_t _pino;
    bool _ligadoTimer;
    unsigned long _tempoInicio;
    bool _ligado;
public:
    Atuador(int pino) : _pino(pino), _ligadoTimer(false), _ligado(false){pinMode(_pino, OUTPUT);};
    virtual ~Atuador(){
    }

    virtual void ligar(){
        digitalWrite(_pino, 1);
    }
    virtual void desligar(){
        digitalWrite(_pino, 0);
    }

    virtual bool ligaTempo(unsigned long tempo){
        if(!_ligadoTimer){
            _tempoInicio = millis();
            _ligadoTimer = true;
            ligar();
            _ligado = true;
            return true;
        }
        else if (millis() - _tempoInicio >= tempo)
        {
            _ligadoTimer = false;
            desligar();
            _ligado = false;
            return true;
        }
        return true;
    }

    virtual bool desligaTempo(unsigned long tempo){
        if(!_ligadoTimer){
            _tempoInicio = millis();
            _ligadoTimer = true;
            ligar();
            _ligado = true;
            return true;
        }
        else if (millis() - _tempoInicio >= tempo)
        {
            _ligadoTimer = false;
            desligar();
            _ligado = false;
            return true;
        }
        return true;
    }
};



// Classe para controlar um LED
class Led : public Atuador{
  public:
};
