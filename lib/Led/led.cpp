#include "led.h" // Inclui o arquivo de cabeçalho que DECLARA a classe Led
#include <Arduino.h> // Necessário para pinMode, digitalWrite, analogWrite, millis, map

/**
 * @brief Construtor da classe Led.
 * Configura o pino especificado como saída e inicializa o estado do LED.
 * @param p O número do pino ao qual o LED está conectado.
 */
Led::Led(int p) {
  pino = p; // Armazena o número do pino
  pinMode(pino, OUTPUT); // Define o pino como saída
  aceso = false; // Inicializa o LED como apagado
  _intensidade = 0; // Inicializa a intensidade (valor interno, pode ser PWM ou percentual)
}

/**
 * @brief Define o estado do LED como completamente aceso (HIGH) ou completamente apagado (LOW).
 * Esta função usa digitalWrite, portanto, não controla a intensidade (PWM).
 * @param estado_val O estado desejado: HIGH para acender, LOW para apagar.
 */
void Led::estado(int estado_val) {
  digitalWrite(pino, estado_val); // Define o estado digital do pino
  // Atualiza o estado interno 'aceso' com base no comando
  if (estado_val == HIGH) {
    aceso = true;
    _intensidade = 255; // Assume intensidade máxima se ligado diretamente
  } else {
    aceso = false;
    _intensidade = 0; // Assume intensidade zero se desligado diretamente
  }
}

/**
 * @brief Define a intensidade do brilho do LED usando PWM.
 * @param intensidade_val A intensidade desejada, em uma escala de 0 (apagado) a 100 (brilho máximo).
 *                      Este valor será mapeado para a faixa de PWM (0-255).
 */
void Led::intensidade(float intensidade_val){
    // Garante que a intensidade_val esteja dentro dos limites esperados (0-100)
    if (intensidade_val < 0) intensidade_val = 0;
    if (intensidade_val > 100) intensidade_val = 100;

    // Armazena o valor percentual da intensidade (0-100)
    _intensidade = intensidade_val; 
    
    int intensidade_pwm;
    // Mapeia o valor percentual (0-100) para o valor PWM (0-255)
    intensidade_pwm = map(static_cast<long>(intensidade_val), 0, 100, 0, 255); 
    
    analogWrite(pino, intensidade_pwm); // Define o brilho do LED usando PWM

    // Atualiza o estado 'aceso' com base na intensidade PWM
    if (intensidade_pwm > 0)
    {
        aceso = true;
    }else{
        aceso = false;
    }
}

/**
 * @brief Acende o LED com brilho máximo por um período especificado e depois o apaga.
 * Se o LED já estiver aceso por esta função, verifica se o tempo expirou.
 * @param tempo A duração, em milissegundos, pela qual o LED deve permanecer aceso.
 * @return true Se o LED foi ligado ou continua ligado dentro do tempo especificado.
 * @return false Se o tempo expirou e o LED foi desligado.
 */
bool Led::acende_tempo(unsigned long tempo) {
  // Se o LED está atualmente apagado (ou foi ligado por outra função como intensidade())
  // e queremos acendê-lo por um tempo.
  if (!aceso || (aceso && (millis() - tempo_inicial >= tempo))) { // Adicionada condição para reativar se o tempo anterior passou
    digitalWrite(pino, HIGH); // Acende o LED com brilho máximo
    aceso = true; // Marca que o LED está aceso (por esta função)
    _intensidade = 100; // Representa 100% de brilho (conceitualmente)
    tempo_inicial = millis(); // Registra o momento em que o LED foi aceso
    return true; // Indica que o LED foi (re)ligado para o período de tempo
  } else {
    // Se o LED já está aceso por esta função e o tempo ainda não expirou
    if (millis() - tempo_inicial < tempo) {
      return true; // Indica que o LED continua aceso dentro do período
    } else {
      // Se o tempo expirou
      digitalWrite(pino, LOW); // Apaga o LED
      aceso = false; // Marca que o LED está apagado
      _intensidade = 0;
      return false; // Indica que o LED foi desligado após o tempo
    }
  }
}