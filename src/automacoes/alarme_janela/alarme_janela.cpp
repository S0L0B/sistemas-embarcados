// Alarme de porta/janela de emergencia
// Problema: avisar quando a saida de emergencia fica aberta por muito tempo.
// Entrada: chave (simula sensor magnetico de porta).  Saida: buzzer + LED laranja.
// Ligacao da chave: pino com INPUT_PULLUP. Fechada = LOW (ligada ao GND), aberta = HIGH.
// Logica: maquina de estados com 3 estados:
//   FECHADA -> (abriu) ABERTA_TOLERANCIA -> (passou TEMPO_TOLERANCIA_JANELA_MS) ALARME
//   Fechou em qualquer estado -> FECHADA (silencia tudo).
#include <Arduino.h>
#include "../../config.h"
#include "alarme_janela.h"

enum EstadoJanela { JANELA_FECHADA, JANELA_TOLERANCIA, JANELA_ALARME };

static EstadoJanela estado = JANELA_FECHADA;
static unsigned long instanteAbertura = 0;
static unsigned long ultimoBip = 0;
static bool bipLigado = false;

static void silencia() {
  noTone(PIN_BUZZER_JANELA);
  digitalWrite(PIN_LED_JANELA, LOW);
  bipLigado = false;
}

void alarmeJanelaInit() {
  pinMode(PIN_CHAVE_JANELA, INPUT_PULLUP);
  pinMode(PIN_BUZZER_JANELA, OUTPUT);
  pinMode(PIN_LED_JANELA, OUTPUT);
  silencia();
}

void alarmeJanelaLoop() {
  unsigned long agora = millis();
  bool aberta = digitalRead(PIN_CHAVE_JANELA) == HIGH;

  if (!aberta) {
    if (estado != JANELA_FECHADA) {
      estado = JANELA_FECHADA;
      silencia();
      Serial.println(F("[JANELA] Fechada"));
    }
    return;
  }

  // Aberta
  if (estado == JANELA_FECHADA) {
    estado = JANELA_TOLERANCIA;
    instanteAbertura = agora;
    Serial.println(F("[JANELA] Aberta - contando tolerancia"));
  }

  if (estado == JANELA_TOLERANCIA && agora - instanteAbertura >= TEMPO_TOLERANCIA_JANELA_MS) {
    estado = JANELA_ALARME;
    Serial.println(F("[JANELA] ALARME: aberta por muito tempo"));
  }

  if (estado == JANELA_ALARME && agora - ultimoBip >= 300) {
    ultimoBip = agora;
    bipLigado = !bipLigado;
    digitalWrite(PIN_LED_JANELA, bipLigado ? HIGH : LOW);
    if (bipLigado) tone(PIN_BUZZER_JANELA, 1000); else noTone(PIN_BUZZER_JANELA);
  }
}
