// Automacao 6 - Contador de pessoas
// Problema: controlar quantas pessoas estao no escritorio e avisar quando lota.
// Entrada: 2 botoes (entrada e saida, simulam sensores de passagem).
// Saida: display de 7 segmentos TM1637 (contagem) e LED vermelho (lotado).
// Logica: cada clique em ENTRADA soma 1; em SAIDA subtrai 1 (nunca abaixo de 0).
//         Com contagem >= LIMITE_LOTACAO, o LED de lotado acende.
//         Os botoes usam debounce por tempo e deteccao de borda (1 clique = 1 pessoa).
#include <Arduino.h>
#include <TM1637Display.h>
#include "../../config.h"
#include "contador_pessoas.h"

static TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);

struct Botao {
  uint8_t pino;
  bool estavaSolto;            // true = nao pressionado na leitura anterior
  unsigned long ultimaMudanca;
};

static Botao btnEntrada = {PIN_BTN_ENTRADA, true, 0};
static Botao btnSaida   = {PIN_BTN_SAIDA,   true, 0};
static int contagem = 0;

// Retorna true uma unica vez a cada clique (borda de descida, com debounce).
static bool clicou(Botao &b) {
  bool solto = digitalRead(b.pino) == HIGH;   // INPUT_PULLUP: pressionado = LOW
  unsigned long agora = millis();
  if (solto != b.estavaSolto && agora - b.ultimaMudanca >= 50) {
    b.ultimaMudanca = agora;
    b.estavaSolto = solto;
    if (!solto) return true;                  // acabou de ser pressionado
  }
  return false;
}

static void atualiza() {
  display.showNumberDec(contagem, false);
  digitalWrite(PIN_LED_LOTADO, contagem >= LIMITE_LOTACAO ? HIGH : LOW);
  Serial.print(F("[CONTADOR] Pessoas: "));
  Serial.print(contagem);
  Serial.println(contagem >= LIMITE_LOTACAO ? F(" (LOTADO)") : F(""));
}

void contadorPessoasInit() {
  pinMode(PIN_BTN_ENTRADA, INPUT_PULLUP);
  pinMode(PIN_BTN_SAIDA, INPUT_PULLUP);
  pinMode(PIN_LED_LOTADO, OUTPUT);
  display.setBrightness(5);
  contagem = 0;
  atualiza();
}

void contadorPessoasLoop() {
  if (clicou(btnEntrada)) {
    if (contagem < 99) contagem++;
    atualiza();
  }
  if (clicou(btnSaida)) {
    if (contagem > 0) contagem--;
    atualiza();
  }
}
