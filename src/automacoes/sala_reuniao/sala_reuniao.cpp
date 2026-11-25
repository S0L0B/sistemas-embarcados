// Sala de reuniao
// Problema: saber de fora se a sala esta em uso.
// Entrada: sensor PIR (movimento).  Saida: LED verde (livre) e LED vermelho (ocupada).
// Logica: maquina de estados com 2 estados. Qualquer movimento -> OCUPADA.
//         Sem movimento por TEMPO_SEM_MOVIMENTO_MS -> LIVRE.
#include <Arduino.h>
#include "../../config.h"
#include "sala_reuniao.h"

enum EstadoSala { SALA_LIVRE, SALA_OCUPADA };

static EstadoSala estado = SALA_LIVRE;
static unsigned long ultimoMovimento = 0;

static void atualizaLeds() {
  digitalWrite(PIN_LED_LIVRE,   estado == SALA_LIVRE   ? HIGH : LOW);
  digitalWrite(PIN_LED_OCUPADA, estado == SALA_OCUPADA ? HIGH : LOW);
}

void salaReuniaoInit() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED_LIVRE, OUTPUT);
  pinMode(PIN_LED_OCUPADA, OUTPUT);
  estado = SALA_LIVRE;
  atualizaLeds();
}

void salaReuniaoLoop() {
  unsigned long agora = millis();
  bool movimento = digitalRead(PIN_PIR) == HIGH;

  if (movimento) {
    ultimoMovimento = agora;
    if (estado != SALA_OCUPADA) {
      estado = SALA_OCUPADA;
      atualizaLeds();
      Serial.println(F("[REUNIAO] Sala OCUPADA"));
    }
  } else if (estado == SALA_OCUPADA && agora - ultimoMovimento >= TEMPO_SEM_MOVIMENTO_MS) {
    estado = SALA_LIVRE;
    atualizaLeds();
    Serial.println(F("[REUNIAO] Sala LIVRE"));
  }
}
