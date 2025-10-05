// Grupo: Merlot
// Giovanna Oliveira Ferreira Dias RM: 566647
// Maria Laura Pereira Druzeic RM: 566634
// Marianne Mukai Nishikawa RM: 568001


// --- Inclusão de Bibliotecas ---
#include <LiquidCrystal.h> // Inclui a biblioteca para controlar o display LCD.
#include <math.h>          // Inclui a biblioteca para funções matemáticas (como pow e isfinite).
#include <stdio.h>         // Inclui a biblioteca padrão de I/O, necessária para a função sprintf.
#include <string.h>        // Inclui a biblioteca para manipulação de strings em C (como strlen e strcat).

// --- Configuração do Hardware ---

// Inicializa o objeto do LCD informando os pinos do Arduino conectados (RS, E, D4, D5, D6, D7).
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define os pinos analógicos para a leitura dos sensores.
const int LDR_PIN     = A0; // Pino para o LDR (sensor de luminosidade).
const int LM35_PIN    = A1; // Pino para o sensor de temperatura LM35. (COMENTÁRIO CORRIGIDO)
const int UMIDADE_PIN = A2; // Pino para o potenciômetro que simula o sensor de umidade.

// Constantes para cálculos elétricos.
const float VCC = 4.88;     // Tensão de alimentação do circuito (5 Volts).
const int R_FIXED = 10000; // Valor do resistor fixo (10kΩ) usado no divisor de tensão com o LDR.

// Define os pinos digitais para os atuadores (LEDs e Buzzer).
const int LED_VERDE    = 10; // Pino para o LED verde.
const int LED_AMARELO  = 9;  // Pino para o LED amarelo.
const int LED_VERMELHO = 8;  // Pino para o LED vermelho.
const int BUZZER       = 6;  // Pino para o buzzer.

// Define a lógica de acionamento dos LEDs (true = acende com HIGH, false = acende com LOW).
const bool LED_ACTIVE_HIGH = true;

// --- Parâmetros de Funcionamento ---

// Constantes usadas na fórmula para converter a resistência do LDR em Lux.
const float A = 600000.0;
const float B = -1.25;

// Define a faixa ideal de luminosidade e a margem de tolerância para alertas.
const float RECOMENDADO_MIN = 100.0; // Valor mínimo recomendado de Lux.
const float RECOMENDADO_MAX = 200.0; // Valor máximo recomendado de Lux.
const float ALERTA_MARGIN   = 20.0;  // Margem de 20 Lux para a faixa de alerta.

// Define a faixa ideal de umidade e a margem de tolerância.
const int UMIDADE_IDEAL_MIN     = 60; // Umidade ideal mínima em %.
const int UMIDADE_IDEAL_MAX     = 80; // Umidade ideal máxima em %.
const int UMIDADE_ALERTA_MARGIN = 10; // Margem de 10% para a faixa de alerta.

// --- Variáveis Globais de Controle ---

unsigned long tempoAnterior = 0;
const unsigned long intervaloDisplay = 5000;
int estadoDisplay = 0;

// --- Definição de Caracteres Personalizados ---

byte tacaVazia[8] = { B10001, B10001, B01010, B00100, B00100, B00100, B11111, B00000 };
byte tacaMetade[8] = { B10001, B11111, B01110, B00100, B00100, B00100, B11111, B00000 };
byte tacaCheia[8]  = { B11111, B11111, B01110, B00100, B00100, B00100, B11111, B00000 };

// --- Funções Auxiliares ---

void printCentralizado(int linha, const char* texto) {
  int col = (16 - strlen(texto)) / 2;
  if (col < 0) col = 0;
  lcd.setCursor(col, linha);
  lcd.print(texto);
}

float calcularLux(int adc) {
  if (adc <= 1) return 0.0;
  if (adc >= 1022) return 1e6;

  float Vout = adc * (VCC / 1023.0);
  float R_LDR = R_FIXED * (VCC / Vout - 1.0);
  if (R_LDR <= 0.0 || !isfinite(R_LDR)) return 1e6;

  float lux = A * pow(R_LDR, B);
  if (!isfinite(lux) || lux < 0.0) lux = 0.0;
  return lux;
}

void acenderLED(bool verde, bool amarelo, bool vermelho) {
  digitalWrite(LED_VERDE,   LED_ACTIVE_HIGH ? verde    : !verde);
  digitalWrite(LED_AMARELO, LED_ACTIVE_HIGH ? amarelo  : !amarelo);
  digitalWrite(LED_VERMELHO, LED_ACTIVE_HIGH ? vermelho : !vermelho);
}

// Função para ler o sensor LM35 e retornar a temperatura em graus Celsius.
float lerTemperaturaCelsius() {
  int leituraADC = analogRead(LM35_PIN);              // Lê o valor analógico (0-1023) do pino do sensor.
  float tensao = (leituraADC * VCC) / 1023.0;         // Converte a leitura ADC para tensão (0V a 5V).

  // A fórmula para o LM35 é: Temp (°C) = Tensão * 100
  // Isso ocorre porque o sensor gera 10mV (0.01V) para cada grau Celsius.
  float temperatura = tensao * 100.0;
  
  return temperatura;
}


int lerUmidadePct() {
  int leituraADC = analogRead(UMIDADE_PIN);
  int umidadePct = map(leituraADC, 0, 1023, 0, 100);
  return umidadePct;
}

// --- Função de Configuração Inicial (Executada uma vez) ---
void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, tacaVazia);
  lcd.createChar(1, tacaMetade);
  lcd.createChar(2, tacaCheia);

  lcd.clear();
  lcd.setCursor(7, 0); lcd.write(byte(0)); delay(500);
  lcd.setCursor(7, 0); lcd.write(byte(1)); delay(500);
  lcd.setCursor(7, 0); lcd.write(byte(2)); delay(500);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.write(byte(2));
  printCentralizado(0, "Merlot");
  lcd.setCursor(15, 0); lcd.write(byte(2));
  printCentralizado(1, "Bem-vindo(a)!");
  delay(3000);
  lcd.clear();

  tempoAnterior = millis();
  estadoDisplay = 0;

  Serial.begin(9600);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  acenderLED(false, false, false);
  digitalWrite(BUZZER, LOW);
}

// --- Função de Loop Principal (Executada continuamente) ---
void loop() {
  int leituraADC = analogRead(LDR_PIN);
  float lux = calcularLux(leituraADC);
  int luminosidade_pct = map((int)lux, 0, 1000, 0, 100);

  float temperatura = lerTemperaturaCelsius();
  int temperaturaInt = (int)temperatura;

  int umidadePct = lerUmidadePct();

  char linha2_buffer[17];

  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervaloDisplay) {
    tempoAnterior = tempoAtual;
    estadoDisplay = (estadoDisplay + 1) % 3;
    lcd.clear();
  }

  switch (estadoDisplay) {
    case 0:
      printCentralizado(0, "TEMPERATURA");
      sprintf(linha2_buffer, "%d%cC ", temperaturaInt, (char)223);
      if (temperaturaInt < 12) strcat(linha2_buffer, "BAIXA");
      else if (temperaturaInt > 14) strcat(linha2_buffer, "ALTA");
      else strcat(linha2_buffer, "IDEAL");
      printCentralizado(1, linha2_buffer);
      break;

    case 1:
      printCentralizado(0, "LUMINOSIDADE");
      sprintf(linha2_buffer, "%d%% ", luminosidade_pct);
      if (lux < RECOMENDADO_MIN - ALERTA_MARGIN || lux > RECOMENDADO_MAX + ALERTA_MARGIN) {
        strcat(linha2_buffer, "ALM");
      } else if ((lux >= RECOMENDADO_MIN - ALERTA_MARGIN && lux < RECOMENDADO_MIN) ||
                 (lux > RECOMENDADO_MAX && lux <= RECOMENDADO_MAX + ALERTA_MARGIN)) {
        strcat(linha2_buffer, "ALT");
      } else {
        strcat(linha2_buffer, "OK ");
      }
      printCentralizado(1, linha2_buffer);
      break;

    case 2:
      printCentralizado(0, "UMIDADE");
      sprintf(linha2_buffer, "%d%% ", umidadePct);
      if (umidadePct < UMIDADE_IDEAL_MIN - UMIDADE_ALERTA_MARGIN ||
          umidadePct > UMIDADE_IDEAL_MAX + UMIDADE_ALERTA_MARGIN) {
        strcat(linha2_buffer, "ALM");
      } else if ((umidadePct >= UMIDADE_IDEAL_MIN - UMIDADE_ALERTA_MARGIN && umidadePct < UMIDADE_IDEAL_MIN) ||
                 (umidadePct > UMIDADE_IDEAL_MAX && umidadePct <= UMIDADE_IDEAL_MAX + UMIDADE_ALERTA_MARGIN)) {
        strcat(linha2_buffer, "ALT");
      } else {
        strcat(linha2_buffer, "OK ");
      }
      printCentralizado(1, linha2_buffer);
      break;
  }

  if (lux < RECOMENDADO_MIN - ALERTA_MARGIN || lux > RECOMENDADO_MAX + ALERTA_MARGIN) {
    acenderLED(false, false, true);
    digitalWrite(BUZZER, LOW);
    delay(500);
  } else if ((lux >= RECOMENDADO_MIN - ALERTA_MARGIN && lux < RECOMENDADO_MIN) ||
             (lux > RECOMENDADO_MAX && lux <= RECOMENDADO_MAX + ALERTA_MARGIN)) {
    acenderLED(false, true, false);
    digitalWrite(BUZZER, HIGH);
    delay(3000);
    digitalWrite(BUZZER, LOW);
    delay(3000);
  } else {
    acenderLED(true, false, false);
    digitalWrite(BUZZER, LOW);
    delay(500);
  }
}