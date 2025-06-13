//Módulo B
#include "Arduino.h"
#include "maq_estados.h"
#include "led.h"
#include "botao.h"

Led buzzer(3);
Led led1(4);
Led led2(5);
Led led3(6);
Led led4(7);
Led leds[] = {led1, led2, led3, led4};
Botao botao(PULL_UP, 2);

enum Estados{leitura, enviar};
Maq_Estados<Estados> maq_estados(leitura);







void setup(){
    Serial.begin(9600);
}

void loop(){
    switch (maq_estados.estado_atual){
    case leitura:
        

    
        break;
    case enviar:

        break;
    default:
        break;
    }
}