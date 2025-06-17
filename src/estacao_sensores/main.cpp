#include "Wire.h"
#include "Arduino.h"
#include "maq_estados.h"
#include "led.h"
#include "botao.h"
#include "sensores.h"
#include "LiquidCrystal_I2C.h"

#define I2C_ADDR_ESCRAVO 8

typedef struct dados{
    float temperatura;
    float luminosidade;
    bool botao;
} Dados;

// //Módulo A: 
//Variáveis globais
Dados dados = {0, 0, false};
unsigned long tempoAnterior = 0;
float temperaturaInicial = -100;
Led led_temp(3);
Led led_1(5);
Led led_2(6);
Led led_3(11);
Led leds[] = {led_1, led_2, led_3};
LiquidCrystal_I2C lcd(0x27, 20, 4);



//Funções auxiliares
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

enum Estados{botao, menu, leitura};
Maq_Estados<Estados> maq_estados(leitura);

void setup(){
    Serial.begin(9600);
    Wire.begin();
    lcd.init();
    lcd.backlight();

}

void loop(){
  //Estados
  switch (maq_estados.estado_atual){
  case leitura:{
    
    //caso digite algo na Serial ele muda para o menu
    if (Serial.available() > 0)
    {
      maq_estados.mudar_estado(menu);
    }
    //Salvar estado anterior do botão
    bool estado_anterior = dados.botao;
    
    //Requisição de dados i2c a cada 100 ms
    if (millis() - tempoAnterior >= 100) {
      tempoAnterior = millis();

      Wire.requestFrom(I2C_ADDR_ESCRAVO, sizeof(dados));

      if (Wire.available() >= sizeof(dados)) {
        Wire.readBytes((byte*)&dados, sizeof(dados));
        if (temperaturaInicial == -100) {
          temperaturaInicial = dados.temperatura;
        }
      }
    }
    //Atualiza leds luminosidade
    intensidade_arranjo(leds, 100 - (dados.luminosidade*100/1.1), 3);

    //Atualiza led temperatura. Caso a temperatura varie 5 graus, o led estará aceso completamente.
    static int intensidade_led = 0;
    if((dados.temperatura - temperaturaInicial) < 0){
      intensidade_led = 0;
    }
    else if (dados.temperatura - temperaturaInicial > 5){
      intensidade_led = 100;
    }
    else{
      intensidade_led = (dados.temperatura - temperaturaInicial)*20;
    }
    led_temp.intensidade(intensidade_led);

    //Caso o estado do botão seja alterado
    if (dados.botao != estado_anterior){
      maq_estados.mudar_estado(botao);
    }
  }
  break;
  case botao:{ 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Botao: ");
    if (dados.botao){
      lcd.print("ON");
      Serial.println("Botao: On");
    }
    else{
      lcd.print("OFF");
      Serial.println("Botao: OFF");
    }
    maq_estados.mudar_estado(leitura);
  }
  break;
  case menu:{
    unsigned long tempo_menu;
    if (maq_estados.primeira_vez_no_estado_atual()){
      String comando = Serial.readStringUntil('\n');
      comando.trim();

      if (comando.equalsIgnoreCase("t")) {
        Serial.print("Ultima Temp: ");
        Serial.println(dados.temperatura);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(dados.temperatura);
        tempo_menu = 3000;
      }
      else if (comando.equalsIgnoreCase("l")) {
        Serial.print("Luminosidade: ");
        Serial.println(dados.luminosidade);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Luz: ");
        lcd.print(dados.luminosidade);
        tempo_menu = 3000;
      }

      else if (comando.equalsIgnoreCase("o")) {
          Wire.beginTransmission(I2C_ADDR_ESCRAVO);
          Wire.write('L');
          Wire.endTransmission();
          Serial.println("Status: Buzzer ON");
          tempo_menu = 0;
      }
      else if (comando.equalsIgnoreCase("f")) {
          Wire.beginTransmission(I2C_ADDR_ESCRAVO);
          Wire.write('D');
          Wire.endTransmission();
          Serial.println("Status: Buzzer OFF");
          tempo_menu = 0;
      }
      else{
        Serial.println("Comando Invalido");
      }
    }
    if(!maq_estados.contador_tempo(tempo_menu)){
      maq_estados.mudar_estado(botao);
    }
    }
    break;
    default:
    break;
  }
}

// // Módulo B: Estação de sensores

// Led buzzer(8);
// Botao botao(PULL_UP_INTERNO, 11);
// Sensor ldr(7, 1.1, 10, 10, 0, 0);
// Sensor lm35(6, 1.1, 10, 10, 100, 0);

// Dados dados = {0, 0, false};

// enum Estados{comando_externo_buzzer, comando_temperatura_buzzer};
// Maq_Estados<Estados> maq_estados(comando_temperatura_buzzer);

// int contador = 0;
// unsigned long tempo_leitura = 10;
// unsigned long tempo_leitura_anterior = 0;

// char comando; 
// float temperatura_inicial;


// void requisicao(){
//     Wire.write((byte*)&dados, sizeof(Dados));
// }

// void comando_recebido(int qtd){
//     if(Wire.available()) {
//         comando = Wire.read();
//     }
//     switch (comando){
//     case 'L':
//         maq_estados.mudar_estado(comando_externo_buzzer);
//         break;
//     case 'D':
//         maq_estados.mudar_estado(comando_temperatura_buzzer);
//         break;
//     default:
//         break;
//     }
// }

// void setup(){
//     Serial.begin(9600);
//     temperatura_inicial = lm35.calcular_grandeza();
    
//     Wire.begin(I2C_ADDR_ESCRAVO);
//     Wire.onReceive(comando_recebido);
//     Wire.onRequest(requisicao);
// }

// void loop(){
//   botao.leitura();

//   //leitura a cada 10 ms
//   if (millis() - tempo_leitura_anterior >= tempo_leitura){
//     dados.temperatura = lm35.calcular_grandeza();
//     dados.luminosidade = ldr.ler_tensao_media();
//     dados.botao = botao.estado();
//   }

//   //Seleciona se o comando do buzzer está sendo comandado externamente
//   switch (maq_estados.estado_atual){
//     case comando_temperatura_buzzer:{
//       if (dados.temperatura - temperatura_inicial >= 5) {
//           buzzer.estado(1);
//       }
//       else{
//           buzzer.estado(0);
//       }
//     }
//     break;
//     case comando_externo_buzzer:{
//       if (maq_estados.primeira_vez_no_estado_atual()){
//         buzzer.estado(1);
//       }
//     }
//     break;
//     default:
//       break;
//   }
// }