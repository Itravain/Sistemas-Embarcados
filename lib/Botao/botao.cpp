#include <Arduino.h>
#include "botao.h"
Botao::Botao(type_t tipo_t, int porta_t) {
      inverter_porta = true;
      porta = porta_t;
      tipo = tipo_t;
      if (tipo == PULL_UP_INTERNO) {
        pinMode(porta, INPUT_PULLUP);
      } else if (tipo == PULL_UP) {
        pinMode(porta, INPUT);
      }
      else {
        inverter_porta = false;
      }

      lastReading = digitalRead(porta);
      buttonState = lastReading;
      lastButtonState = lastReading;
      lastDebounceTime = millis();
    }

// Realiza a leitura do botão com debounce
void Botao::leitura() {
  reading = digitalRead(porta);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }
  lastReading = reading;
}

// Verifica se o botão foi solto
bool Botao::pressionado_solto() {
  leitura();
  bool pressed = (lastButtonState == LOW && buttonState == HIGH);
  lastButtonState = buttonState;
  return pressed;
}

// Verifica se o botão foi pressionado
bool Botao::botao_pressionado() {
  leitura();
  bool pressed = (lastButtonState == HIGH && buttonState == LOW);
  lastButtonState = buttonState;
  return pressed;
}

bool Botao::estado() {
  if (inverter_porta) {
    return (buttonState+1)%2;
  }
  return buttonState;
}