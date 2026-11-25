# Escritório Inteligente (simulação)

sete automações no mesmo ambiente (um escritório), simuladas no Wokwi com Arduino Mega 2560.

## Automações

| # | Automação | Entrada | Saída | Lógica |
|---|-----------|---------|-------|--------|
| 1 | Sala de reunião ocupada/livre | Sensor PIR | LED verde e LED vermelho | Máquina de estados com timeout sem movimento |
| 2 | Sala de servidores | Sensor DHT22 (temperatura) | Relé (ventilador) e buzzer | Máquina de estados com histerese (NORMAL, ALERTA, CRÍTICO) |
| 3 | Controle de acesso | Teclado matricial 4x4 | Servo (porta) e LCD | Máquina de estados com senha, bloqueio após 3 erros |
| 4 | Iluminação por luz ambiente | LDR | LED amarelo PWM | Brilho inversamente proporcional à luz, com filtro |
| 5 | Alarme de porta/janela de emergência | Chave (slide switch) | Buzzer e LED laranja | Máquina de estados: tolerância de 5 s e depois alarme |
| 6 | Contador de pessoas | 2 botões (entrada e saída) | Display TM1637 e LED vermelho de lotação | Contagem com debounce e limite de lotação (5) |
| 7 | Controle de umidade | Sensor DHT22 (umidade) | Relé (umidificador) e LED azul | Histerese: liga o umidificador em baixa umidade e desliga em alta umidade |

## Estrutura

```
platformio.ini      configuração da placa e bibliotecas
wokwi.toml          liga o Wokwi ao firmware compilado
diagram.json        circuito do Wokwi
src/config.h        pinos e parâmetros
src/main.cpp        chama as 7 automações
src/sala_reuniao.*  automação 1
src/sala_servidores.* automação 2
src/controle_acesso.* automação 3
src/iluminacao.*    automação 4
src/alarme_janela.* automação 5
src/contador_pessoas.* automação 6
src/controle_umidade.*    automação 7
```

## Como executar (VS Code)

1. Instale as extensões **PlatformIO IDE** e **Wokwi Simulator** (a Wokwi pede uma licença gratuita na primeira vez: `F1` > "Wokwi: Request a New License").
2. Abra esta pasta no VS Code.
3. Compile: botão **Build** do PlatformIO (ou `pio run`).
4. Rode a simulação: `F1` > **Wokwi: Start Simulator**.
5. Abra o monitor serial do Wokwi para ver os logs.

## Como testar cada automação

1. **Reunião:** clique no PIR durante a simulação e use a opção de simular movimento. LED vermelho acende; 10 s sem movimento volta ao verde.
2. **Servidores:** clique no DHT22 e mude a temperatura. Em 28 °C o relé liga; em 35 °C o buzzer apita; abaixo de 33 °C o alarme para; abaixo de 26 °C o relé desliga.
3. **Acesso:** senha `1234` seguida de `#` abre a porta (servo em 90°) por 5 s. `*` apaga. Três erros bloqueiam por 15 s.
4. **Iluminação:** mude o slider de lux do LDR. Mais escuro, LED mais forte. Se estiver invertido, troque `LDR_VALOR_ALTO_E_CLARO` em `src/config.h`.
5. **Janela:** a chave começa fechada. Ao abri-la, o alarme só soa após 5 s (buzzer e LED laranja piscando); ao fechar, silencia. Se o sentido da chave estiver invertido, vire a chave para o outro lado.
6. **Contador:** o botão verde soma uma pessoa e o vermelho subtrai (mínimo 0). Com 5 pessoas ou mais o LED vermelho acende.
7. **Caixa d'água:** ajuste a distância no HC-SR04. A partir de 80 cm (caixa baixa) a bomba liga; a 30 cm ou menos (caixa cheia) desliga. Entre 30 e 80 cm o estado se mantém.
