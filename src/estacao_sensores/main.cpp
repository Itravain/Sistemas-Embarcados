//Módulo B
#include "Wire.h"
#include "Arduino.h"
#include "maq_estados.h"
#include "led.h"
#include "botao.h"
#include "sensores.h"

Led buzzer(8);
Botao botao(PULL_UP_INTERNO, 11);
Sensor ldr(7, 1.1, 10, 10, 0, 0);
Sensor lm35(6, 1.1, 10, 10, 100, 0);

enum Estados{leitura, enviar};
Maq_Estados<Estados> maq_estados(leitura);

int contador = 0;
unsigned long tempo_leitura = 10;
unsigned long tempo_envio = 100;
bool mostrar = false;
float temperatura_inicial;
float temperatura_atual;


void setup(){
    Serial.begin(9600);
    temperatura_inicial = lm35.calcular_grandeza();
    Wire.begin();
}

void loop(){
    botao.leitura();
    Serial.println(botao.estado());
    switch (maq_estados.estado_atual){
    case leitura:
        if (maq_estados.contador_tempo(tempo_leitura))
        {
            ldr.ler_tensao_media();
            temperatura_atual = lm35.calcular_grandeza();
            if (temperatura_atual >= temperatura_inicial+5)
            {
                buzzer.estado(1);
            }
            else{
                buzzer.estado(0);
            } 
            contador++;
        }
        if(contador*tempo_leitura >= tempo_envio){
            contador = 0;
            maq_estados.mudar_estado(enviar);
        }        
        break;
    case enviar:
        Serial.println("Enviado");

        mostrar = false;
        maq_estados.mudar_estado(leitura);
        break;
    default:
        break;
    }
}