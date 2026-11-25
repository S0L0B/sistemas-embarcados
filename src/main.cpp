
// Todas usam millis() (sem delay), entao rodam "ao mesmo tempo".
#include <Arduino.h>
#include "automacoes/sala_reuniao/sala_reuniao.h"
#include "automacoes/sala_servidores/sala_servidores.h"
#include "automacoes/controle_acesso/controle_acesso.h"
#include "automacoes/iluminacao/iluminacao.h"
#include "automacoes/alarme_janela/alarme_janela.h"
#include "automacoes/contador_pessoas/contador_pessoas.h"
#include "automacoes/controle_umidade/controle_umidade.h"

void setup() {
  Serial.begin(9600);
  Serial.println(F("Escritorio Inteligente - iniciando"));
  salaReuniaoInit();
  salaServidoresInit();
  controleAcessoInit();
  iluminacaoInit();
  alarmeJanelaInit();
  contadorPessoasInit();
  controleUmidadeInit();
}

void loop() {
  salaReuniaoLoop();
  salaServidoresLoop();
  controleAcessoLoop();
  iluminacaoLoop();
  alarmeJanelaLoop();
  contadorPessoasLoop();
  controleUmidadeLoop();
}
