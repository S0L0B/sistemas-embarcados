//Iluminacao por luz ambiente
// Problema: manter o escritorio bem iluminado sem desperdicar energia.
// Entrada: LDR (leitura analogica).  Saida: LED amarelo com brilho PWM (luminaria).
// Logica: quanto mais escuro o ambiente, maior o brilho da luminaria (proporcao inversa).
//         A leitura passa por um filtro de media movel exponencial para evitar tremulacao.
#include <Arduino.h>
#include "../../config.h"
#include "iluminacao.h"

static unsigned long ultimaLeitura = 0;
static float filtrado = 0;
static unsigned long ultimoLog = 0;

void iluminacaoInit() {
  pinMode(PIN_LED_LUZ, OUTPUT);
  filtrado = analogRead(PIN_LDR);
}

void iluminacaoLoop() {
  unsigned long agora = millis();
  if (agora - ultimaLeitura < INTERVALO_LDR_MS) return;
  ultimaLeitura = agora;

  int leitura = analogRead(PIN_LDR);              // 0..1023
  filtrado = 0.8f * filtrado + 0.2f * leitura;    // suaviza

  // Converte a leitura em "claridade" de 0 (escuro) a 1023 (claro).
  int claridade = LDR_VALOR_ALTO_E_CLARO ? (int)filtrado : 1023 - (int)filtrado;

  // Escuro -> LED forte; claro -> LED apagado.
  int brilho = map(claridade, 0, 1023, 255, 0);
  brilho = constrain(brilho, 0, 255);
  analogWrite(PIN_LED_LUZ, brilho);

  if (agora - ultimoLog >= 1000) {                // log a cada 1 s
    ultimoLog = agora;
    Serial.print(F("[LUZ] LDR: "));
    Serial.print(leitura);
    Serial.print(F(" | Brilho PWM: "));
    Serial.println(brilho);
  }
}
