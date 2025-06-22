#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pines de botones y LEDs
const int buttonPlusPin = 18;
const int buttonMinusPin = 16;
const int ledGreenPin = 21;
const int ledRedPin = 48;

// Dirección y tamaño pantalla LCD I2C SSD2004A (20 columnas, 4 filas)
#define LCD_ADDRESS 0x3F  // Cambia si es necesario
#define LCD_COLUMNS 20
#define LCD_ROWS 4

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// Límite máximo y variable del contador
const int maxCount = 30;
int count = 0;

// Estados anteriores de los botones
bool lastButtonPlusState = HIGH;
bool lastButtonMinusState = HIGH;

// Declaración anticipada de funciones
void mostrarEstado();
void checkButtonPlus();
void checkButtonMinus();
void actualizarIndicadores();

void setup() {
  Serial.begin(115200);

  // Configurar botones como entrada con resistencia pull-up interna
  pinMode(buttonPlusPin, INPUT_PULLUP);
  pinMode(buttonMinusPin, INPUT_PULLUP);

  // Configurar LEDs como salida
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);

  // Apagar LEDs al iniciar
  digitalWrite(ledGreenPin, LOW);
  digitalWrite(ledRedPin, LOW);

  // Inicializar pantalla LCD I2C
  Wire.begin(); // Usar pines SDA/SCL por defecto del microcontrolador
  lcd.init();
  lcd.backlight();

  mostrarEstado();  // Mostrar el valor inicial
}

void loop() {
  checkButtonPlus();
  checkButtonMinus();
  actualizarIndicadores();
}

void checkButtonPlus() {
  int reading = digitalRead(buttonPlusPin);

  if (reading == LOW && lastButtonPlusState == HIGH) {
    if (count < maxCount) {
      count++;
      mostrarEstado();
    } else {
      Serial.println("Lleno");
    }
  }

  lastButtonPlusState = reading;
}

void checkButtonMinus() {
  int reading = digitalRead(buttonMinusPin);

  if (reading == LOW && lastButtonMinusState == HIGH) {
    if (count > 0) {
      count--;
      mostrarEstado();
    }
  }

  lastButtonMinusState = reading;
}

void actualizarIndicadores() {
  if (count < maxCount) {
    digitalWrite(ledGreenPin, HIGH);
    digitalWrite(ledRedPin, LOW);
  } else {
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledRedPin, HIGH);
  }
}

void mostrarEstado() {
  Serial.print("Conteo actual: ");
  Serial.println(count);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conteo:");

  lcd.setCursor(0, 1);
  lcd.print(count);
}
