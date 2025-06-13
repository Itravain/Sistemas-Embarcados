#include <Arduino.h>

// Enum para definir os níveis de dificuldade do jogo
typedef enum dificuldade{FACIL, MEDIO, DIFICIL, INDEFINIDA} dificuldade_t;

// Enum para definir o tipo de configuração do botão
typedef enum type{PULL_UP, PULL_DOWN, PULL_UP_INTERNO} type_t;

// Enum para definir os estados da máquina de estados do jogo
typedef enum estados{menu_inicial, menu_interm, menu_jogo, game_over, timer_finalizado, recebe_letra,
   letra_correta, letra_errada, palavra_incorreta, recebe_palavra, parabens} estados_t;

// Banco de dados de palavras
#define QTD_PALAVRAS 30
String *banco_dados = new String[QTD_PALAVRAS]{
  "saudade", "lua", "pessoa", "beijo", "aluno", "gato", "gato", "beijo", 
  "triste", "flor", "tempo", "cheiro", "casa", "carro", "feliz", "escola", 
  "paz", "falar", "casa", "corpo", "saudade", "flor", "corpo", "falar", 
  "mundo", "corpo", "saudade", "cheiro", "cheiro", "janela"
};

// Classe para controlar um LED
class Led {
  private:
    bool aceso; // Indica se o LED está aceso logicamente
    uint64_t intensidade; 

  public:
    int pino; // Pino do Arduino ao qual o LED está conectado
    unsigned long tempo_inicial; // Tempo inicial para controle de temporização

    // Construtor: inicializa o pino do LED
    Led(int p) {
      pino = p;
      pinMode(pino, OUTPUT);
      aceso = false;
      intensidade = 0;
    }

    // Define o estado do LED (HIGH ou LOW)
    void estado(int estado) {
      digitalWrite(pino, estado);
    }

    // Acende o LED por um tempo especificado
    bool acende_tempo(unsigned long tempo) {
      if (!aceso) {
        digitalWrite(pino, HIGH);
        aceso = true;
        tempo_inicial = millis();
        return true;
      } else {
        if (millis() - tempo_inicial > tempo) {
          digitalWrite(pino, LOW);
          aceso = false;
          return false;
        } else {
          return true;
        }
      }
    }
};

// Classe para gerenciar a máquina de estados do jogo
class Maq_Estados {
  private:
    bool contando; // Indica se o temporizador está ativo
    unsigned long tempo_inicial; // Tempo inicial para o temporizador
    bool controle; // Controle para execução única em um estado

  public:
    estados_t estado_atual; // Estado atual da máquina de estados

    // Construtor: inicializa o estado inicial
    Maq_Estados() {
      estado_atual = menu_inicial;
      controle = false;
      contando = false;
    }

    // Muda o estado atual da máquina
    void mudar_estado(estados_t prox_estado) {
      estado_atual = prox_estado;
      controle = false;
    }

    // Temporizador não bloqueante
    bool contador_tempo(unsigned long tempo) {
      if (!contando) {
        contando = true;
        tempo_inicial = millis();
        return true;
      } else {
        if (millis() - tempo_inicial >= tempo) {
          contando = false;
          return false;
        } else {
          return true;
        }
      }
    }

    // Verifica se é a primeira vez que o estado está sendo executado
    bool primeira_vez_estado() {
      if (!controle) {
        controle = true;
        return true;
      } else {
        return false;
      }
    }
};

// Classe para gerenciar um botão com debounce
class Botao {
  public:
    int porta; // Pino do Arduino ao qual o botão está conectado
    type_t tipo; // Tipo de configuração do botão
    int reading; // Leitura atual do botão
    int lastReading; // Última leitura do botão
    int buttonState; // Estado estável do botão
    unsigned long lastDebounceTime; // Última vez que o botão mudou de estado
    unsigned long debounceDelay = 20; // Tempo de debounce em milissegundos
    int lastButtonState; // Último estado do botão

    // Construtor: inicializa o botão
    Botao(type_t tipo_t, int porta_t) {
      porta = porta_t;
      tipo = tipo_t;
      if (tipo == PULL_UP_INTERNO) {
        pinMode(porta, INPUT_PULLUP);
      } else if (tipo == PULL_UP) {
        pinMode(porta, INPUT);
      }
      lastReading = digitalRead(porta);
      buttonState = lastReading;
      lastButtonState = lastReading;
      lastDebounceTime = millis();
    }

    // Realiza a leitura do botão com debounce
    void leitura() {
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
    bool pressionado_solto() {
      leitura();
      bool pressed = (lastButtonState == LOW && buttonState == HIGH);
      lastButtonState = buttonState;
      return pressed;
    }

    // Verifica se o botão foi pressionado
    bool botao_pressionado() {
      leitura();
      bool pressed = (lastButtonState == HIGH && buttonState == LOW);
      lastButtonState = buttonState;
      return pressed;
    }
};

// Classe principal do jogo da forca
class Jogo {
  public:
    bool jogo_iniciado; // Indica se o jogo está em andamento
    int tentativas; // Número de tentativas restantes
    unsigned long tempo_atual; // Tempo atual
    unsigned long tempo_inicial; // Tempo inicial do jogo
    unsigned long duracao; // Duração do jogo
    unsigned int indice_palavra_atual; // Índice da palavra atual
    String palavras_atuais[3]; // Palavras sorteadas para o jogo
    dificuldade_t dificuldade = INDEFINIDA; // Dificuldade selecionada
    bool letras_ja_escolhidas[27]; // Letras já escolhidas pelo jogador
    String palavra_display[3]; // Representação da palavra com underscores
    // Construtor: inicializa o jogo
    Jogo() {
      tentativas = 5;
      jogo_iniciado = false;
    }

    // Inicializa o jogo com base na dificuldade
    void inicializar() {
      jogo_iniciado = true;
      tempo_inicial = millis();
      if (dificuldade == FACIL) duracao = 120 * 1000UL;
      else if (dificuldade == MEDIO) duracao = 60 * 1000UL;
      else if (dificuldade == DIFICIL) duracao = 30 * 1000UL;
      tentativas = 5;

      for (int i = 0; i < dificuldade + 1; i++) {
        palavras_atuais[i] = banco_dados[random(0, QTD_PALAVRAS)];
        palavra_display[i] = criar_palavra_display(palavras_atuais[i].length());
      }
      indice_palavra_atual = 0;
    }

    // Finaliza o jogo
    void finalizarJogo() {
      jogo_iniciado = false;
      dificuldade = INDEFINIDA;
    }

    // Cria a string de underscores para a palavra
    String criar_palavra_display(unsigned int tamanho) {
      String retorno;
      retorno.reserve(tamanho * 2 - 1);
      for (unsigned int i = 0; i < tamanho - 1; i++) {
        retorno += "_.";
      }
      retorno += "_";
      return retorno;
    }

    // Imprime o menu do jogo
    void imprimir_menu() {
      Serial.println("\n\n-------------------------------------------");
      Serial.print("Palavra ");
      Serial.print(indice_palavra_atual + 1);
      Serial.print(": ");
      Serial.println(palavra_display[indice_palavra_atual]);
      Serial.print("Tentativas: ");
      Serial.println(tentativas);
      Serial.println("Pressione o botão da opção desejada: \n(1) Chutar a palavra completa \n(2) Chutar uma letra na palavra");
      Serial.println("-------------------------------------------\n");
    }

    // Processa o chute de uma letra
    void encontra_letra(char letra) {
      bool achou = false;
      for (unsigned int j = 0; j < palavras_atuais[indice_palavra_atual].length(); j++) {
        if (palavras_atuais[indice_palavra_atual][j] == letra) {
          palavra_display[indice_palavra_atual][j * 2] = letra;
          achou = true;
        }
      }
      if (!achou) {
        Serial.println("Letra incorreta!");
        tentativas--;
      } else {
        Serial.println("Letra correta!");
      }
      tempo_inicial = millis();
    }

    // Processa o chute de uma palavra
    void encontra_palavra(String palavra_chute) {
      if (palavra_chute == palavras_atuais[indice_palavra_atual]) {
        Serial.println("Palavra correta!");
        for (unsigned long i = 0; i < palavras_atuais[indice_palavra_atual].length(); i++) {
          palavra_display[indice_palavra_atual][i * 2] = palavras_atuais[indice_palavra_atual][i];
        }
      } else {
        Serial.println("Palavra Incorreta!");
        tentativas--;
      }
      tempo_inicial = millis();
    }

    // Verifica se ainda há tempo
    bool tem_tempo() {
      if (jogo_iniciado) {
        tempo_atual = millis();
        if (tempo_atual - tempo_inicial < duracao) {
          return true;
        } else {
          Serial.println("Tempo estourado!");
          tentativas--;
          tempo_inicial = millis();
          return false;
        }
      }
      return true;
    }

    // Verifica se a palavra foi completada
    bool verificacao_palavra() {
      for (unsigned int j = 0; j < palavras_atuais[indice_palavra_atual].length(); j++) {
        if (palavra_display[indice_palavra_atual][j * 2] == '_') {
          return false;
        }
      }
      return true;
    }

    // Verifica se o jogador ganhou
    bool ganhou() {
      if (!verificacao_palavra()) {
        return false;
      } else {
        if (indice_palavra_atual < dificuldade) {
          indice_palavra_atual++;
          tentativas = 5;
          return false;
        } else {
          return true;
        }
      }
    }
};

// Instâncias globais
Jogo jogo;
Maq_Estados m_estados;
Botao botao1(PULL_UP_INTERNO, 3);
Botao botao2(PULL_UP_INTERNO, 4);
Botao botao3(PULL_UP_INTERNO, 5);
Led vermelho(9);
Led amarelo(10);
Led verde(11);
Led buzzer(8);

// Configuração inicial
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

// Loop principal
void loop() {
  // Atualiza os estados dos botões e LEDs
  botao1.leitura();
  botao2.leitura();
  botao3.leitura();
  vermelho.estado(!botao1.buttonState);
  amarelo.estado(!botao2.buttonState);
  verde.estado(!botao3.buttonState);

  // Verifica se o tempo acabou
  if (!jogo.tem_tempo()) {
    m_estados.mudar_estado(menu_jogo);
  }

  // Máquina de estados principal
  switch (m_estados.estado_atual) {
    case menu_inicial:
      if (m_estados.primeira_vez_estado()) {
        Serial.print("Pressione o botão para a opção desejada:\n(1) Dificuldade 'fácil'\n(2) Dificuldade 'média'\n(3) Dificuldade 'difícil'\n");
        Serial.println("-------------------------------------------");
      }
      if (botao1.pressionado_solto()) {
        jogo.dificuldade = FACIL;
      } else if (botao2.pressionado_solto()) {
        jogo.dificuldade = MEDIO;
      } else if (botao3.pressionado_solto()) {
        jogo.dificuldade = DIFICIL;
      }
      if (jogo.dificuldade != INDEFINIDA) {
        m_estados.mudar_estado(menu_interm);
      }
      break;

    case menu_interm:
      if (m_estados.primeira_vez_estado()) {
        Serial.println("Pressione o botão para a opção desejada: \n(1) Iniciar o jogo\n(2) Voltar");
        Serial.println("-------------------------------------------");
      }
      if (botao1.pressionado_solto()) {
        m_estados.mudar_estado(menu_jogo);
        jogo.inicializar();
      } else if (botao2.pressionado_solto()) {
        m_estados.mudar_estado(menu_inicial);
        jogo.dificuldade = INDEFINIDA;
      }
      break;

    case menu_jogo:
      if (jogo.tentativas == 0) {
        Serial.print("O jogo acabou! A palavra era: ");
        Serial.println(jogo.palavras_atuais[jogo.indice_palavra_atual]);
        jogo.finalizarJogo();
        m_estados.mudar_estado(game_over);
      } else if (jogo.ganhou()) {
        m_estados.mudar_estado(parabens);
      } else {
        if (m_estados.primeira_vez_estado()) {
          jogo.imprimir_menu();
        }
        if (botao1.pressionado_solto()) {
          m_estados.mudar_estado(recebe_palavra);
        }
        if (botao2.pressionado_solto()) {
          m_estados.mudar_estado(recebe_letra);
        }
      }
      break;

    case game_over:
      if (m_estados.primeira_vez_estado()) {
        Serial.println("Game Over!\n-----------------------------------\n\n");
      }
      vermelho.estado(1);
      verde.estado(1);
      amarelo.estado(1);
      buzzer.estado(1);
      if (!m_estados.contador_tempo(2000)) {
        vermelho.estado(0);
        verde.estado(0);
        amarelo.estado(0);
        buzzer.estado(0);
        m_estados.mudar_estado(menu_inicial);
      }
      break;

    case recebe_letra:
      if (m_estados.primeira_vez_estado()) {
        Serial.println("\nDigite uma letra\n");
      }
      if (Serial.available() > 0) {
        char letra = Serial.read();
        jogo.encontra_letra(letra);
        m_estados.mudar_estado(menu_jogo);
      }
      break;

    case recebe_palavra:
      if (m_estados.primeira_vez_estado()) {
        Serial.println("\nDigite uma palavra\n");
      }
      if (Serial.available() > 0) {
        String palavra = Serial.readStringUntil('\n');
        jogo.encontra_palavra(palavra);
        m_estados.mudar_estado(menu_jogo);
      }
      break;

    case parabens:
      if (m_estados.primeira_vez_estado()) {
        Serial.println("Parabéns, você ganhou!\n");
        Serial.println("\n\n-------------------------------------------");
        jogo.finalizarJogo();
      }
      vermelho.estado(1);
      verde.estado(1);
      amarelo.estado(1);
      if (!m_estados.contador_tempo(2000)) {
        vermelho.estado(0);
        verde.estado(0);
        amarelo.estado(0);
        m_estados.mudar_estado(menu_inicial);
      }
      break;

    default:
      break;
  }
}