#include <Arduino.h>
#include "led.h"
#include "maq_estados.h"

enum Estados{inicializacao, leitura};
Maq_Estados<Estados> maq_estados(inicializacao);

Led led1(3);
Led led2(5);
Led led3(6);
Led led4(10);
Led buzzer(2);
Led leds[] = {led1, led2, led3, led4};
int numero_leds = sizeof(leds) / sizeof(leds[0]);
unsigned long tempo = 20;


void intensidade_arranjo(Led leds[], float intensidade_geral, int numero_leds) {
  if (numero_leds <= 0) {
    return;
  }
  float intervalo_intensidade_led = 100 / numero_leds;
  for (int i = 0; i < numero_leds; i++)
  {
    float valor_minimo_led = i * intervalo_intensidade_led;
    float valor_max_led = (i+1) * intervalo_intensidade_led;

    if(intensidade_geral > valor_max_led){
      leds[i].estado(HIGH);
    }else if (intensidade_geral < valor_minimo_led)
    {
      leds[i].estado(LOW);
    }
    else{
      float valor_led_atual = map(intensidade_geral, valor_minimo_led, valor_max_led, 0, 100);
      leds[i].intensidade(valor_led_atual);
    } 
  }
}

float temperatura(){
  float tensao = 1.1/1024.0 * analogRead(A0);
  float temperatura = 100 * tensao;
  return temperatura;
}

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
}

void loop() {
  static float temperatura_inicial = 0; // Declare as static to retain value between calls
  switch (maq_estados.estado_atual){
  case inicializacao: {
    temperatura_inicial = temperatura();
    Serial.println(temperatura_inicial);
    maq_estados.mudar_estado(leitura);
    break;
  }
  case leitura: {
    float temperatura_atual = temperatura();
    Serial.println(temperatura_atual);
    float temp_alerta = temperatura_inicial + 6;
    float sinal_led = map(temperatura_atual, temperatura_inicial, temp_alerta, 100, 0);
    intensidade_arranjo(leds, sinal_led, 4);
    
    if(temperatura_atual > temp_alerta){
      buzzer.estado(1);
      delay(1000);
      buzzer.estado(0);
    };
    break;
  }
  default:
    break;
  }
}