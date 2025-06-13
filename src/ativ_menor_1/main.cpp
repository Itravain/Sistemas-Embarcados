#include <Arduino.h>

int controle = 0;
unsigned long millisAnterior = 0;
int estado = 0;
int senhaPadrao = 1234;
int senha;
int pinoBuzzer = 4;
char opcao;

void setup() {
  Serial.begin(9600);
  pinMode(pinoBuzzer, OUTPUT);
}

void loop() {
  unsigned long millisAtual = millis();

  switch (estado){
    case 0: //Idle
      if(controle == 0){
        Serial.write("Digite a senha para acessar o menu principal!\n");
        controle = 1;
      }
      if(Serial.available()>0){
        senha = Serial.parseInt();
        if(senha == senhaPadrao){
          estado = 2;
          controle = 0;
        }
        else{
          estado = 1;
          controle = 0;
        }
      }
    break;
    case 1: //Buzzer tocando
      if(controle == 0){
        millisAnterior = millis();
        digitalWrite(pinoBuzzer, 1);
        Serial.write("Senha incorreta\n");
        controle = 1;
      }
      if(millis()-millisAnterior >= 1000){
        digitalWrite(pinoBuzzer, 0);
        controle = 0;
        estado = 0;
      }
    break;
    case 2: //Menu
      if(controle == 0){
        Serial.write("Senha correta, bem-vindo!\n\nPressione (a) para alterar a senha\nPressione (b) para visualizar a senha atual\nPressione (c) para voltar para o início.\n\n");
        controle = 1;
      }
      if(Serial.available()>0){
        opcao = Serial.read();
        if(opcao == 'a'){
          estado = 3;
          controle = 0;
        }
        else if(opcao == 'b'){
          estado = 4;
          controle = 0;
        }
        else if(opcao == 'c'){
          estado = 0;
          controle = 0;
        }
      }
    break;
    case 3: //Alterar a senha
      if(controle == 0){
        Serial.write("Digite a nova senha\n");
        controle = 1;
      }
      if(Serial.available()>0){
        senhaPadrao = Serial.parseInt();
        Serial.write("Senha alterada com sucesso\n");
        estado = 2;
        controle = 0;
      }
    break;
    case 4: //Ver a senha
      Serial.print("A sua senha é: ");
      Serial.print(senha);
      Serial.write("\n");
      estado = 2;
    break;
  }




}
