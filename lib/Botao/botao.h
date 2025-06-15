#ifndef BOTAO_H
#define BOTAO_H

#include <Arduino.h>

// Enum para definir o tipo de configuração do botão
typedef enum type { PULL_UP, PULL_DOWN, PULL_UP_INTERNO } type_t;

// Classe para gerenciar um botão com debounce
class Botao {
  public:
  int buttonState; // Estado estável do botão
  
  // Construtor: inicializa o botão
  Botao(type_t tipo_t, int porta_t);
  
  // Realiza a leitura do botão com debounce
  void leitura();
  
  // Verifica se o botão foi solto
  bool pressionado_solto();
  
  // Verifica se o botão foi pressionado
  bool botao_pressionado();

  // Retorna o estado do botao naquele instante
  bool estado(); 
  
private:
  unsigned long lastDebounceTime; // Última vez que o botão mudou de estado
  unsigned long debounceDelay; // Tempo de debounce em milissegundos
  int lastButtonState; // Último estado do botão
  int porta; // Pino do Arduino ao qual o botão está conectado
  type_t tipo; // Tipo de configuração do botão
  int reading; // Leitura atual do botão
  int lastReading; // Última leitura do botão
  bool inverter_porta;
};

#endif // BOTAO_H