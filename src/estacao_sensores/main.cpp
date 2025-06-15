#include "Wire.h"
#include "Arduino.h"
#include "maq_estados.h"
#include "led.h"
#include "botao.h"
#include "sensores.h"

#define I2C_ADDR_ESCRAVO 8

typedef struct dados{
    float temperatura;
    float luminosidade;
    bool botao;
} Dados;

//Módulo A: 
//Variáveis globais
Dados dados = {0, 0, false};
unsigned long tempoAnterior = 0;
float temperaturaInicial = -100;
Led led_ldr(3);
Led led_1(5);
Led led_2(6);
Led led_3(11);
Led leds[] = {led_1, led_2, led_3};

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

void verificarMenuSerial() {
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando.equalsIgnoreCase("Temperatura")) {
      Serial.print("Ultima Temp: ");
      Serial.println(dados.temperatura);
    //   lcd.clear();
    //   lcd.setCursor(0, 0);
    //   lcd.print("Temp: ");
    //   lcd.print(dadosRecebidos.temperatura);
    }
    // ... implementar outros comandos do menu...
    else if (comando.equalsIgnoreCase("LigarBuzzer")) {
        Wire.beginTransmission(I2C_ADDR_ESCRAVO);
        Wire.write('L');
        Wire.endTransmission();
        Serial.println("Status: Buzzer ON");
    }
    else if (comando.equalsIgnoreCase("DesligarBuzzer")) {
        Wire.beginTransmission(I2C_ADDR_ESCRAVO);
        Wire.write('D');
        Wire.endTransmission();
        Serial.println("Status: Buzzer OFF");
    }
  }
}




void setup(){
    Serial.begin(9600);
    Wire.begin();
}

void loop(){
  if (millis() - tempoAnterior >= 100) {
    tempoAnterior = millis();

    Wire.requestFrom(I2C_ADDR_ESCRAVO, sizeof(dados));

    if (Wire.available() >= sizeof(dados)) {
      Wire.readBytes((byte*)&dados, sizeof(dados));

        if (temperaturaInicial == -100) {
        temperaturaInicial = dados.temperatura;
        }
        
        Serial.println(dados.temperatura);
        Serial.println(dados.luminosidade);
        Serial.println(dados.botao);
    
        intensidade_arranjo(leds, 100 - (dados.luminosidade*100/1.1), 3);
    // atualizarLedTemperatura();
    // atualizarDisplay();
    }
  }

//   verificarMenuSerial();
}


//Módulo B: Estação de sensores
/*
Led buzzer(8);
Botao botao(PULL_UP_INTERNO, 11);
Sensor ldr(7, 1.1, 10, 10, 0, 0);
Sensor lm35(6, 1.1, 10, 10, 100, 0);

Dados dados = {0, 0, false};

enum Estados{leitura, enviar};
Maq_Estados<Estados> maq_estados(leitura);

int contador = 0;
unsigned long tempo_leitura = 10;
unsigned long tempo_leitura_anterior = 0;

char comando; 
float temperatura_inicial;


void requisicao(){
    Wire.write((byte*)&dados, sizeof(Dados));
}

void comando_recebido(int qtd){
    if(Wire.available()) {
        comando = Wire.read();
    }
    switch (comando){
    case 'L':
        buzzer.estado(1);
        break;
    case 'D':
        buzzer.estado(0);
        break;
    default:
        break;
    }
}

void setup(){
    Serial.begin(9600);
    temperatura_inicial = lm35.calcular_grandeza();
    
    Wire.begin(I2C_ADDR_ESCRAVO);
    Wire.onReceive(comando_recebido);
    Wire.onRequest(requisicao);
}

void loop(){
    botao.leitura();
    //Buzzer
    if (dados.temperatura - temperatura_inicial >= 5) {
        buzzer.estado(1);
    }
    else{
        buzzer.estado(0);
    }
    
    //leitura a cada 10 ms
    if (millis() - tempo_leitura_anterior >= tempo_leitura){
        dados.temperatura = lm35.calcular_grandeza();
        dados.luminosidade = ldr.ler_tensao_media();
        dados.botao = botao.estado();
        Serial.println(dados.temperatura);
        Serial.println(dados.luminosidade);
        Serial.println(dados.botao);
    }
}
*/



