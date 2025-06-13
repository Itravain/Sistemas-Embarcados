# Projeto de Sistemas Embarcados

Este repositório contém os projetos e bibliotecas desenvolvidos para a disciplina de Sistemas Embarcados. O objetivo é fornecer uma base de código modular e reutilizável para diversas atividades práticas utilizando a plataforma PlatformIO.

## Visão Geral

O projeto está organizado para facilitar o desenvolvimento de múltiplos subprojetos (atividades) que compartilham um conjunto comum de bibliotecas. A estrutura principal é gerenciada pelo PlatformIO, simplificando a compilação, o upload e o gerenciamento de dependências para microcontroladores.

## Estrutura do Projeto

```
Projetos/
├── platformio.ini       # Arquivo de configuração principal do PlatformIO
├── README.md            # Este arquivo
├── include/             # Arquivos de cabeçalho globais (se necessário)
│   └── README           # Instruções para a pasta include
├── lib/                 # Bibliotecas customizadas compartilhadas entre os projetos
│   ├── README           # Instruções para a pasta lib
│   ├── Atuadores/       # Biblioteca para controle de atuadores genéricos
│   │   ├── atuadores.cpp
│   │   └── atuadores.h  # (Presumindo que exista ou será criado)
│   ├── Botao/           # Biblioteca para gerenciamento de botões
│   │   ├── botao.cpp
│   │   └── botao.h
│   ├── Fila/            # Biblioteca para implementação de uma estrutura de dados de fila
│   │   ├── fila.cpp
│   │   └── fila.h
│   ├── Led/             # Biblioteca para controle de LEDs
│   │   ├── led.cpp
│   │   └── led.h
│   ├── Maquina_estados/ # Biblioteca para implementação de máquinas de estados
│   │   └── maq_estados.h
│   └── Sensores/        # Biblioteca para leitura e processamento de sensores
│       ├── sensores.cpp
│       └── sensores.h
├── src/                 # Código fonte dos projetos/atividades individuais
│   ├── ativ_menor_1/
│   │   └── main.cpp
│   ├── estacao_sensores/ # Projeto exemplo: Estação de Sensores
│   │   └── main.cpp
│   └── forca/            # Projeto exemplo: Jogo da Forca
│       └── main.cpp
└── test/                # Testes unitários (se aplicável)
    └── README           # Instruções para a pasta test
```

## Configuração do Ambiente

1.  **Instale o VS Code:** Baixe e instale o [Visual Studio Code](https://code.visualstudio.com/).
2.  **Instale a extensão PlatformIO IDE:** No VS Code, vá para a aba de Extensões (Ctrl+Shift+X) e procure por "PlatformIO IDE" e instale-a.
3.  **Clone este repositório:**
    ```bash
    git clone <URL_DO_SEU_REPOSITORIO>
    cd Projetos
    ```
4.  **Abra o projeto no VS Code:**
    *   Com o VS Code aberto, clique em "File" > "Open Folder..." e selecione a pasta `Projetos` que você clonou.
    *   O PlatformIO deve reconhecer o projeto automaticamente.

## Como Compilar e Rodar um Projeto Existente

Cada subpasta dentro de `src/` representa um projeto ou atividade independente que pode ser compilado e carregado no microcontrolador de destino.

Para compilar e carregar um projeto específico (por exemplo, `estacao_sensores`):

1.  **Modifique o `platformio.ini` (se necessário):**
    O `platformio.ini` na raiz do projeto é configurado para compilar **todos** os projetos em `src/` por padrão, o que pode não ser o desejado se você quiser focar em um único `main.cpp`.
    Para focar em um projeto específico, você pode comentar ou ajustar as configurações de `src_filter` no seu `platformio.ini`.

    Alternativamente, e de forma mais comum para múltiplos `main.cpp` em um único projeto PlatformIO, você pode criar diferentes **ambientes** no `platformio.ini`, cada um apontando para um `src_dir` diferente.

    Exemplo de `platformio.ini` para múltiplos projetos como ambientes distintos:

    ```ini
    [platformio]
    default_envs = esp32dev_estacao_sensores ; Defina o ambiente padrão aqui

    [env]
    platform = espressif32 ; Ou a plataforma do seu microcontrolador
    board = esp32dev       ; Ou o modelo da sua placa
    framework = arduino
    monitor_speed = 115200
    lib_deps =
        ; Dependências comuns a todos os ambientes, se houver

    [env:esp32dev_estacao_sensores]
    src_dir = src/estacao_sensores/
    build_src_filter = +<*> -<../**/main.cpp> +<main.cpp> ; Garante que apenas o main.cpp de estacao_sensores seja compilado

    [env:esp32dev_forca]
    src_dir = src/forca/
    build_src_filter = +<*> -<../**/main.cpp> +<main.cpp>

    ; Adicione mais ambientes conforme necessário
    ```
    Com esta configuração:
    *   Para compilar e carregar o projeto `estacao_sensores`, selecione o ambiente `esp32dev_estacao_sensores` na barra de status do PlatformIO no VS Code ou use o comando:
        ```bash
        platformio run -e esp32dev_estacao_sensores --target upload
        ```
    *   Para compilar e carregar o projeto `forca`:
        ```bash
        platformio run -e esp32dev_forca --target upload
        ```

2.  **Usando a Interface do PlatformIO no VS Code:**
    *   Clique no ícone do PlatformIO (cabeça de formiga) na barra lateral esquerda.
    *   Em "PROJECT TASKS", expanda o ambiente desejado (ex: `esp32dev_estacao_sensores`).
    *   Clique em "Build" para compilar ou "Upload" para compilar e carregar na placa.
    *   Use "Monitor" para abrir o Serial Monitor.

## Como Criar um Novo Projeto (Atividade)

1.  **Crie uma nova pasta:** Dentro do diretório `src/`, crie uma nova pasta para sua atividade. Por exemplo, `src/minha_nova_atividade/`.
2.  **Adicione seu `main.cpp`:** Crie um arquivo `main.cpp` dentro da nova pasta (`src/minha_nova_atividade/main.cpp`).
    ```cpp
    #include <Arduino.h>
    // Inclua as bibliotecas necessárias de lib/
    #include "Led/led.h" // Exemplo

    // Defina seus objetos e lógica
    Led meuLed(13); // Exemplo

    void setup() {
      Serial.begin(115200);
      meuLed.acender();
    }

    void loop() {
      meuLed.piscar(500);
      delay(1000);
    }
    ```
3.  **Adicione um novo ambiente no `platformio.ini`:**
    Copie um ambiente existente e modifique o `src_dir` e o nome do ambiente.
    ```ini
    [env:esp32dev_minha_nova_atividade] ; Use um nome de ambiente descritivo
    src_dir = src/minha_nova_atividade/
    build_src_filter = +<*> -<../**/main.cpp> +<main.cpp>
    ; Copie outras configurações relevantes como platform, board, framework do [env] base
    platform = ${env.platform}
    board = ${env.board}
    framework = ${env.framework}
    monitor_speed = ${env.monitor_speed}
    lib_deps =
        ${env.lib_deps}
    ```
4.  **Selecione o ambiente:** No VS Code, selecione o novo ambiente na barra de status do PlatformIO ou use o comando CLI especificando o ambiente (`-e nome_do_ambiente`).
5.  **Compile e Carregue:** Use as opções "Build" e "Upload" do PlatformIO.

## Como Utilizar as Bibliotecas da Pasta `lib/`

As bibliotecas na pasta `lib/` são automaticamente detectadas pelo PlatformIO e podem ser incluídas nos seus arquivos `main.cpp` ou em outras bibliotecas.

**Exemplo de Inclusão:**

Para usar a biblioteca `Led` em um `main.cpp`:
```cpp
#include <Arduino.h>
#include "Led/led.h" // Caminho relativo à pasta lib

Led meuLed(D4); // Supondo que D4 seja o pino do LED

void setup() {
  meuLed.acender();
}

void loop() {
  meuLed.piscar(1000); // Pisca a cada 1 segundo
  delay(2000);
  meuLed.apagar();
  delay(2000);
}
```

**Visão Geral das Bibliotecas:**

*   **`Atuadores`**: (Ainda a ser detalhada) Provavelmente para controle de motores, servos, etc.
*   **`Botao`**: Facilita a leitura de botões, tratando debouncing e diferentes tipos de pressionamento (curto, longo).
    ```cpp
    #include "Botao/botao.h"
    Botao meuBotao(D1, PULL_UP); // Pino D1, resistor de pull-up interno
    // ... no loop ...
    // if (meuBotao.foiPressionado()) { /* ação */ }
    ```
*   **`Fila`**: Implementação de uma estrutura de dados de fila, útil para bufferizar dados ou eventos.
    ```cpp
    #include "Fila/fila.h"
    fila minhaFila;
    // minhaFila.push(10);
    // int valor = minhaFila.pop();
    // int media = minhaFila.media();
    ```
*   **`Led`**: Abstração para controlar LEDs (acender, apagar, piscar).
    ```cpp
    #include "Led/led.h"
    Led ledVerde(D2);
    // ledVerde.acender();
    // ledVerde.apagar();
    // ledVerde.piscar(500); // ms
    ```
*   **`Maquina_estados`**: Template para criar máquinas de estados finitos, ajudando a organizar lógicas complexas.
    ```cpp
    #include "Maquina_estados/maq_estados.h"
    // enum MeusEstados {ESTADO_A, ESTADO_B};
    // Maq_Estados<MeusEstados> minhaMaquina(ESTADO_A);
    // ... no loop, switch(minhaMaquina.estado_atual) ...
    ```
*   **`Sensores`**: (Ainda a ser detalhada) Provavelmente para leitura e processamento de dados de diversos tipos de sensores.

Consulte os arquivos `.h` e `.cpp` de cada biblioteca para entender completamente sua API e funcionalidades.
