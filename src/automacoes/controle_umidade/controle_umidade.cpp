// Controle de umidade
// Problema: manter o ambiente com umidade confortavel.
// Entrada: sensor DHT22.
// Saida: rele de umidificador e LED azul.
// Logica: umidade baixa liga o umidificador e umidade alta desliga, com histerese.
#include <Arduino.h>
#include <DHT.h>
#include "../../config.h"
#include "controle_umidade.h"

static DHT dhtUmidade(PIN_DHT_UMIDADE, DHT22);
static unsigned long ultimaLeitura = 0;
static bool umidificadorLigado = false;

static void umidificador(bool ligar) {
  umidificadorLigado = ligar;
  digitalWrite(PIN_RELE_UMIDIFICADOR,
               ligar == RELE_UMIDIFICADOR_ATIVO_ALTO ? HIGH : LOW);
  digitalWrite(PIN_LED_UMIDADE, ligar ? HIGH : LOW);
}

void controleUmidadeInit() {
  dhtUmidade.begin();
  pinMode(PIN_RELE_UMIDIFICADOR, OUTPUT);
  pinMode(PIN_LED_UMIDADE, OUTPUT);
  umidificador(false);
}

void controleUmidadeLoop() {
  unsigned long agora = millis();
  if (agora - ultimaLeitura < INTERVALO_UMIDADE_MS) return;
  ultimaLeitura = agora;

  float umidade = dhtUmidade.readHumidity();
  if (isnan(umidade)) {
    Serial.println(F("[UMIDADE] Falha na leitura do DHT22"));
    return;
  }

  if (!umidificadorLigado && umidade <= UMIDADE_LIGA) umidificador(true);
  else if (umidificadorLigado && umidade >= UMIDADE_DESLIGA) umidificador(false);

  Serial.print(F("[UMIDADE] Umidade: "));
  Serial.print(umidade, 0);
  Serial.print(F(" % | Umidificador: "));
  Serial.println(umidificadorLigado ? F("LIGADO") : F("DESLIGADO"));
}
