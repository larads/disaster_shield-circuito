#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>

// Definições de Pinos
#define redPin 8
#define greenPin 9
#define bluePin 6
#define buzzer 5
#define echo_sens 3
#define trig_sens 2
#define sig_soil A0
#define display_sda A4
#define display_scl A5

// Liquid Crystal
#define col 16   // Serve para definir o numero de colunas do display utilizado
#define row 2    // Serve para definir o numero de linhas do display utilizado
#define adr 0x27 // Serve para definir o endereço do display.

// Declarando Limites
const int maxHeightMeters = 3;
#define idealHeightMeters 1.5
#define criticHeightMeters 2.5
#define driedSoil 400
#define wetSoil 650

// Declaração dos estados
void (*PonteiroDeFuncao)();
void getMeasures(void);
void checkEnviromentState(void);
void setAlarms(void);
void displayInfos(void);

// Declaração de variáveis
LiquidCrystal_I2C lcd(adr, col, row);
const int numeroAmostras = 100; // Número de amostras coletadas no solo.
float currentDistance;
int avgUmidSoil;
bool isFloodCritical;
bool isSlipCritical;
bool isFloodAlert;
bool isSlipAlert;

// Definindo as Funções de estado
void getMeasures(void)
{
    // Distância do Sensor Ultrassônico
    currentDistance = getCurrentDistance(); // pega a distância atual

    // Sensor de Solo
    avgUmidSoil = getUmidSoil();

    PonteiroDeFuncao = checkEnviromentState;
}

void checkEnviromentState(void)
{
    isFloodCritical = false;
    isSlipCritical = false;
    isFloodAlert = false;
    isSlipAlert = false;

    float distanceRelative = maxHeightMeters - currentDistance;
    char distanceStr[10];                         // espaço suficiente para "999.9m\0"
    dtostrf(distanceRelative, 3, 1, distanceStr); // converte float para string com 1 casa decimal

    char distanceRelStr[17];
    snprintf(distanceRelStr, sizeof(distanceRelStr) - 1, "Dist: %s m", distanceStr);

    // String para o Solo
    char umidAvgStr[10];
    itoa(avgUmidSoil, umidAvgStr, 10); // base 10

    char umidStr[17];
    snprintf(umidStr, sizeof(umidStr), "Solo Umid. %s", umidAvgStr);

    // Definição de Estado da distância
    char soilLevel[17];
    if (distanceRelative <= idealHeightMeters)
    {
        snprintf(soilLevel, sizeof(soilLevel), "Nivel IDEAL");
    }
    else if (distanceRelative > criticHeightMeters)
    {
        snprintf(soilLevel, sizeof(soilLevel), "Nivel CRITICO");
        isFloodCritical = true;
    }
    else
    {
        snprintf(soilLevel, sizeof(soilLevel), "Nivel OK");
        isFloodAlert = true;
    }

    writeOnDisplay(distanceRelStr, soilLevel, 500);

    // Definição de Estado do solo
    char umidLevel[17];
    if (avgUmidSoil < driedSoil)
    {
        snprintf(umidLevel, sizeof(umidLevel), "Solo SECO");
    }
    else if (avgUmidSoil > wetSoil)
    {
        snprintf(umidLevel, sizeof(umidLevel), "Solo MOLHADO");
        isSlipCritical = true;
    }
    else
    {
        snprintf(umidLevel, sizeof(umidLevel), "Solo UMIDO");
        isSlipAlert = true;
    }
    writeOnDisplay(umidStr, umidLevel, 500);

    if (isFloodCritical || isSlipCritical || isFloodAlert || isSlipAlert)
        PonteiroDeFuncao = setAlarms;
    else
    {
        PonteiroDeFuncao = getMeasures;
        setLedColor(0, 255, 0);
    }
}

void setAlarms(void)
{
    // Aqui irá colocar a cor do led RGB em vermelho e ligar o buzzer
    if (isFloodCritical)
    {
        setLedColor(255, 0, 0);
        writeOnDisplayWithoutClear("Risco iminente", "de ENCHENTES");
        playBuzzer(1000, 1500);
        lcd.clear();
    }
    if (isFloodAlert)
    {
        setLedColor(255, 255, 0);
        writeOnDisplay("Sinal proximo", "de ENCHENTES", 800);
    }

    if (isSlipCritical)
    {
        setLedColor(255, 0, 0);
        writeOnDisplayWithoutClear("Risco iminente", "de DESLIZAMENTO");
        playBuzzer(1000, 1500);
        lcd.clear();
    }
    if (isSlipAlert)
    {
        setLedColor(255, 255, 0);
        writeOnDisplay("Sinal proximo", "de DESLIZAMENTO", 800);
    }

    PonteiroDeFuncao = getMeasures;
}

// Início do Código
void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(sig_soil, INPUT);
    pinMode(trig_sens, OUTPUT);
    pinMode(echo_sens, INPUT);

    Serial.begin(9600);
    writeOnDisplay("    Disaster Shield    ", "SIMULA ENCHENTES", 800);
    PonteiroDeFuncao = getMeasures;
}

void loop()
{
    (*PonteiroDeFuncao)();
}

// Funções de Apoio ao Código
void writeOnDisplay(char msg1[], char msg2[], int interval)
{
    lcd.setCursor(0, 0);
    lcd.print(String(msg1));
    lcd.setCursor(0, 1);
    lcd.print(String(msg2));
    delay(interval);
    lcd.clear();
}
void writeOnDisplayWithoutClear(char msg1[], char msg2[])
{
    lcd.setCursor(0, 0);
    lcd.print(String(msg1));
    lcd.setCursor(0, 1);
    lcd.print(String(msg2));
}

float getCurrentDistance()
{
    long time;
    float distance;

    digitalWrite(trig_sens, LOW);
    delayMicroseconds(2);

    digitalWrite(trig_sens, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_sens, LOW);

    // Calculando distância
    time = pulseIn(echo_sens, HIGH);
    distance = time * 0.0343 / 2;

    return distance / 100;
}

int getUmidSoil()
{
    long sumAmostras = 0; // Somatória das amostras

    for (int i = 0; i < numeroAmostras; i++)
    {
        int readSensor = analogRead(sig_soil);
        sumAmostras += readSensor;
    }
    int media = sumAmostras / numeroAmostras;

    return media;
}

void setLedColor(int red, int green, int blue)
{
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
}

void playBuzzer(int khz, int interval)
{
    tone(buzzer, khz);
    delay(interval);
    noTone(buzzer);
}