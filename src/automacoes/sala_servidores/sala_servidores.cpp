// Sala de servidores
// Problema: evitar superaquecimento dos equipamentos.
// Entrada: sensor de temperatura DHT22.  Saida: rele (ventilador) e buzzer (alarme).
// Logica: maquina de estados com histerese para nao ficar ligando/desligando:
//   NORMAL  -> ventilador desligado
//   ALERTA  -> ventilador ligado (temp >= TEMP_LIGA_VENT, volta abaixo de TEMP_DESLIGA_VENT)
//   CRITICO -> ventilador ligado + buzzer intermitente (temp >= TEMP_ALARME_ON,
//              volta abaixo de TEMP_ALARME_OFF)
#include <Arduino.h>
#include <DHT.h>
#include "../../config.h"
#include "sala_servidores.h"

enum EstadoServ { SERV_NORMAL, SERV_ALERTA, SERV_CRITICO };

static DHT dht(PIN_DHT, DHT22);
static EstadoServ estado = SERV_NORMAL;
static unsigned long ultimaLeitura = 0;
static unsigned long ultimoBip = 0;
static bool bipLigado = false;

static void ventilador(bool ligar) {
  digitalWrite(PIN_RELE, (ligar == RELE_ATIVO_ALTO) ? HIGH : LOW);
}

void salaServidoresInit() {
  pinMode(PIN_RELE, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  dht.begin();
  ventilador(false);
  noTone(PIN_BUZZER);
}

void salaServidoresLoop() {
  unsigned long agora = millis();

  // Leitura periodica do sensor (o DHT22 nao deve ser lido mais rapido que ~0,5 Hz).
  if (agora - ultimaLeitura >= INTERVALO_LEITURA_DHT_MS) {
    ultimaLeitura = agora;
    float t = dht.readTemperature();
    if (isnan(t)) {
      Serial.println(F("[SERVIDORES] Falha na leitura do DHT22"));
    } else {
      EstadoServ anterior = estado;
      switch (estado) {
        case SERV_NORMAL:
          if (t >= TEMP_ALARME_ON)       estado = SERV_CRITICO;
          else if (t >= TEMP_LIGA_VENT)  estado = SERV_ALERTA;
          break;
        case SERV_ALERTA:
          if (t >= TEMP_ALARME_ON)         estado = SERV_CRITICO;
          else if (t < TEMP_DESLIGA_VENT)  estado = SERV_NORMAL;
          break;
        case SERV_CRITICO:
          if (t < TEMP_ALARME_OFF) estado = SERV_ALERTA;
          break;
      }
      ventilador(estado != SERV_NORMAL);
      if (estado != SERV_CRITICO) { noTone(PIN_BUZZER); bipLigado = false; }

      Serial.print(F("[SERVIDORES] Temp: "));
      Serial.print(t, 1);
      Serial.print(F(" C | Estado: "));
      Serial.println(estado == SERV_NORMAL ? F("NORMAL")
                   : estado == SERV_ALERTA ? F("ALERTA") : F("CRITICO"));
      (void)anterior;
    }
  }

  // Buzzer intermitente enquanto estiver em estado critico (sem usar delay).
  if (estado == SERV_CRITICO && agora - ultimoBip >= 400) {
    ultimoBip = agora;
    bipLigado = !bipLigado;
    if (bipLigado) tone(PIN_BUZZER, 2000); else noTone(PIN_BUZZER);
  }
}
