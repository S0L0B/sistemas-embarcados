
// Placa: Arduino Mega 2560. 
#pragma once
#include <Arduino.h>

// ---------- Automacao 1: Sala de reuniao (PIR + LEDs) ----------
constexpr uint8_t PIN_PIR          = 22;
constexpr uint8_t PIN_LED_LIVRE    = 23;  
constexpr uint8_t PIN_LED_OCUPADA  = 24;   
constexpr unsigned long TEMPO_SEM_MOVIMENTO_MS = 10000;  

// ---------- Automacao 2: Sala de servidores (DHT22 + rele + buzzer) ----------
constexpr uint8_t PIN_DHT          = 25;
constexpr uint8_t PIN_RELE         = 26;   // aciona o ventilador/exaustor
constexpr uint8_t PIN_BUZZER       = 27;
constexpr bool    RELE_ATIVO_ALTO  = true; // se o rele ligar invertido, troque para false
constexpr float TEMP_LIGA_VENT     = 28.0; // liga ventilador
constexpr float TEMP_DESLIGA_VENT  = 26.0; // desliga ventilador (histerese)
constexpr float TEMP_ALARME_ON     = 35.0; // entra em alarme critico
constexpr float TEMP_ALARME_OFF    = 33.0; // sai do alarme (histerese)
constexpr unsigned long INTERVALO_LEITURA_DHT_MS = 2000;

// ---------- Automacao 3: Controle de acesso (teclado + LCD + servo) ----------
constexpr uint8_t PIN_SERVO        = 9;
constexpr uint8_t LINHAS_TECLADO   = 4;
constexpr uint8_t COLUNAS_TECLADO  = 4;
constexpr uint8_t PINOS_LINHAS[LINHAS_TECLADO]   = {30, 31, 32, 33};
constexpr uint8_t PINOS_COLUNAS[COLUNAS_TECLADO] = {34, 35, 36, 37};
constexpr uint8_t LCD_ENDERECO     = 0x27;
constexpr char    SENHA_CORRETA[]  = "1234";
constexpr uint8_t TAMANHO_SENHA    = 4;
constexpr uint8_t TENTATIVAS_MAX   = 3;
constexpr unsigned long PORTA_ABERTA_MS = 5000;
constexpr unsigned long BLOQUEIO_MS     = 15000;
constexpr uint8_t ANGULO_FECHADA   = 0;
constexpr uint8_t ANGULO_ABERTA    = 90;

// ---------- Automacao 4: Iluminacao por luz ambiente (LDR + LED PWM) ----------
constexpr uint8_t PIN_LDR          = A0;
constexpr uint8_t PIN_LED_LUZ      = 4;    // pino PWM (timer0; os pinos 2, 3 e 5 ficam livres para o tone())
// No modulo LDR do Wokwi, confira no monitor serial se mais luz = valor maior ou menor
// e ajuste esta flag. true: valor ALTO = ambiente CLARO.
constexpr bool    LDR_VALOR_ALTO_E_CLARO = true;
constexpr unsigned long INTERVALO_LDR_MS = 100;

// ---------- Automacao 5: Alarme de porta/janela de emergencia (chave + buzzer + LED) ----------
constexpr uint8_t PIN_CHAVE_JANELA  = 40;  // INPUT_PULLUP: LOW = fechada, HIGH = aberta
constexpr uint8_t PIN_BUZZER_JANELA = 28;
constexpr uint8_t PIN_LED_JANELA    = 29;  // LED laranja
constexpr unsigned long TEMPO_TOLERANCIA_JANELA_MS = 5000;  // 5 s antes de soar o alarme

// ---------- Automacao 6: Contador de pessoas (2 botoes + TM1637 + LED) ----------
constexpr uint8_t PIN_BTN_ENTRADA   = 42;  // INPUT_PULLUP
constexpr uint8_t PIN_BTN_SAIDA     = 43;  // INPUT_PULLUP
constexpr uint8_t PIN_TM_CLK        = 44;
constexpr uint8_t PIN_TM_DIO        = 45;
constexpr uint8_t PIN_LED_LOTADO    = 46;  // LED vermelho
constexpr int     LIMITE_LOTACAO    = 5;

// ---------- Automacao 7: Controle de umidade (DHT22 + rele + LED) ----------
constexpr uint8_t PIN_DHT_UMIDADE   = 47;
constexpr uint8_t PIN_RELE_UMIDIFICADOR = 49;
constexpr uint8_t PIN_LED_UMIDADE  = 50;  // LED azul
constexpr bool    RELE_UMIDIFICADOR_ATIVO_ALTO = true;
constexpr float UMIDADE_LIGA       = 40.0;  // liga umidificador
constexpr float UMIDADE_DESLIGA    = 60.0;  // desliga com histerese
constexpr unsigned long INTERVALO_UMIDADE_MS = 2000;
