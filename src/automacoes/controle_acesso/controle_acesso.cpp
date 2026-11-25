// Controle de acesso
// Problema: permitir a entrada no escritorio so de quem sabe a senha.
// Entrada: teclado matricial 4x4.  Saida: servo (trava da porta) e display LCD.
// Teclas: 0-9 digitam, '#' confirma, '*' apaga o que foi digitado.
// Logica: maquina de estados:
//   ESPERA -> (1a tecla) DIGITANDO -> (#) confere a senha
//     certa  -> ABERTA (servo abre, fecha sozinho apos PORTA_ABERTA_MS) -> ESPERA
//     errada -> conta erro; ao atingir TENTATIVAS_MAX -> BLOQUEADA por BLOQUEIO_MS -> ESPERA
#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "../../config.h"
#include "controle_acesso.h"

enum EstadoAcesso { ACESSO_ESPERA, ACESSO_DIGITANDO, ACESSO_ABERTA, ACESSO_BLOQUEADA };

static const char TECLAS[LINHAS_TECLADO][COLUNAS_TECLADO] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

static Keypad teclado = Keypad(makeKeymap(TECLAS),
                               const_cast<byte*>(PINOS_LINHAS),
                               const_cast<byte*>(PINOS_COLUNAS),
                               LINHAS_TECLADO, COLUNAS_TECLADO);
static LiquidCrystal_I2C lcd(LCD_ENDERECO, 16, 2);
static Servo porta;

static EstadoAcesso estado = ACESSO_ESPERA;
static char digitado[TAMANHO_SENHA + 1];
static uint8_t tamanho = 0;
static uint8_t erros = 0;
static unsigned long instanteEstado = 0;

static void mostra(const char* linha1, const char* linha2 = "") {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linha1);
  lcd.setCursor(0, 1);
  lcd.print(linha2);
}

static void limpaDigitado() {
  tamanho = 0;
  digitado[0] = '\0';
}

static void mostraAsteriscos() {
  char linha[TAMANHO_SENHA + 1];
  for (uint8_t i = 0; i < tamanho; i++) linha[i] = '*';
  linha[tamanho] = '\0';
  mostra("Digite a senha:", linha);
}

static void voltaParaEspera() {
  estado = ACESSO_ESPERA;
  limpaDigitado();
  porta.write(ANGULO_FECHADA);
  mostra("Acesso restrito", "Digite a senha");
}

void controleAcessoInit() {
  lcd.init();
  lcd.backlight();
  porta.attach(PIN_SERVO);
  voltaParaEspera();
}

void controleAcessoLoop() {
  unsigned long agora = millis();

  // Estados temporizados (nao dependem de tecla).
  if (estado == ACESSO_ABERTA && agora - instanteEstado >= PORTA_ABERTA_MS) {
    Serial.println(F("[ACESSO] Porta fechada"));
    voltaParaEspera();
  }
  if (estado == ACESSO_BLOQUEADA) {
    if (agora - instanteEstado >= BLOQUEIO_MS) {
      erros = 0;
      Serial.println(F("[ACESSO] Bloqueio terminou"));
      voltaParaEspera();
    }
    return;  // ignora teclas durante o bloqueio
  }
  if (estado == ACESSO_ABERTA) return;

  char tecla = teclado.getKey();
  if (!tecla) return;

  if (tecla == '*') {                       // apagar
    limpaDigitado();
    voltaParaEspera();
  } else if (tecla == '#') {                // confirmar
    if (tamanho == TAMANHO_SENHA && strcmp(digitado, SENHA_CORRETA) == 0) {
      erros = 0;
      estado = ACESSO_ABERTA;
      instanteEstado = agora;
      porta.write(ANGULO_ABERTA);
      mostra("Acesso liberado", "Porta aberta");
      Serial.println(F("[ACESSO] Senha correta - porta aberta"));
    } else {
      erros++;
      limpaDigitado();
      Serial.print(F("[ACESSO] Senha incorreta. Erros: "));
      Serial.println(erros);
      if (erros >= TENTATIVAS_MAX) {
        estado = ACESSO_BLOQUEADA;
        instanteEstado = agora;
        mostra("Acesso bloqueado", "Aguarde...");
      } else {
        estado = ACESSO_ESPERA;
        mostra("Senha incorreta", "Tente novamente");
      }
    }
  } else if (tecla >= '0' && tecla <= '9') {  // digitar
    if (tamanho < TAMANHO_SENHA) {
      digitado[tamanho++] = tecla;
      digitado[tamanho] = '\0';
      estado = ACESSO_DIGITANDO;
      mostraAsteriscos();
    }
  }
}
