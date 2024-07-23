#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int ledPinStanga = 4;
const int ledPinDreapta = 7;
const int buttonPinStanga = 3;
const int buttonPinDreapta = 6;
const int buttonPinAmbele = 8;
bool ledStateStanga = false;
bool ledStateDreapta = false;
bool ledStateAmbele = false;
bool buttonStateStanga = false;
bool lastButtonStateStanga = false;
bool buttonStateDreapta = false;
bool lastButtonStateDreapta = false;
bool buttonStateAmbele = false;
bool lastButtonStateAmbele = false;
unsigned long previousMillisStanga = 0;
unsigned long previousMillisDreapta = 0;
const long interval = 500; // Intervalul pentru licărire (500ms)

void setup() {
  lcd.begin();
  pinMode(buttonPinStanga, INPUT_PULLUP);
  pinMode(buttonPinDreapta, INPUT_PULLUP);
  pinMode(buttonPinAmbele, INPUT_PULLUP);
  pinMode(ledPinStanga, OUTPUT);
  pinMode(ledPinDreapta, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Citire starea butoanelor
  buttonStateStanga = digitalRead(buttonPinStanga) == LOW;
  buttonStateDreapta = digitalRead(buttonPinDreapta) == LOW;
  buttonStateAmbele = digitalRead(buttonPinAmbele) == LOW;

  // Gestionare semnalizare stanga
  if (buttonStateStanga && !lastButtonStateStanga) {
    ledStateStanga = !ledStateStanga; // Comută starea LED-ului
    if (ledStateStanga) {
      lcd.setCursor(1, 0); // Setare pozitie mesaj prima coloana
      lcd.print("Semnalizare"); // Trimitere mesaj
      lcd.setCursor(1, 1); // Setare pozitie mesaj a doua coloana
      lcd.print("Stanga"); // Trimitere mesaj
    } else {
      buttonStangaStins();
    }
  }
  lastButtonStateStanga = buttonStateStanga;

  // Gestionare semnalizare dreapta
  if (buttonStateDreapta && !lastButtonStateDreapta) {
    ledStateDreapta = !ledStateDreapta; // Comută starea LED-ului
    if (ledStateDreapta) {
      lcd.setCursor(1, 0); // Setare pozitie mesaj prima coloana
      lcd.print("Semnalizare"); // Trimitere mesaj
      lcd.setCursor(1, 1); // Setare pozitie mesaj a doua coloana
      lcd.print("Dreapta"); // Trimitere mesaj
    } else {
      buttonDreaptaStins();
    }
  }
  lastButtonStateDreapta = buttonStateDreapta;

  // Gestionare semnalizare ambele
  if (buttonStateAmbele && !lastButtonStateAmbele) {
    ledStateAmbele = !ledStateAmbele; // Comută starea LED-ului
    if (ledStateAmbele) {
      lcd.setCursor(5, 0); // Setare pozitie mesaj prima coloana
      lcd.print("Avarii"); // Trimitere mesaj
    } else {
      buttonAmbeleStins();
    }
  }
  lastButtonStateAmbele = buttonStateAmbele;

  // Licărire LED stânga
  if ((ledStateStanga || ledStateAmbele) && currentMillis - previousMillisStanga >= interval) {
    previousMillisStanga = currentMillis;
    int ledState = digitalRead(ledPinStanga);
    digitalWrite(ledPinStanga, !ledState); // Comută starea LED-ului
  }

  // Licărire LED dreapta
  if ((ledStateDreapta || ledStateAmbele) && currentMillis - previousMillisDreapta >= interval) {
    previousMillisDreapta = currentMillis;
    int ledState = digitalRead(ledPinDreapta);
    digitalWrite(ledPinDreapta, !ledState); // Comută starea LED-ului
  }

  delay(50); // Un mic delay pentru debounce
}

void buttonStangaStins() {
  digitalWrite(ledPinStanga, LOW); // Oprește LED-ul stânga
  if (!ledStateDreapta && !ledStateAmbele) lcd.clear(); // Șterge ecranul dacă nici dreapta și nici ambele nu sunt active
}

void buttonDreaptaStins() {
  digitalWrite(ledPinDreapta, LOW); // Oprește LED-ul dreapta
  if (!ledStateStanga && !ledStateAmbele) lcd.clear(); // Șterge ecranul dacă nici stânga și nici ambele nu sunt active
}

void buttonAmbeleStins() {
  digitalWrite(ledPinStanga, LOW); // Oprește LED-ul stânga
  digitalWrite(ledPinDreapta, LOW); // Oprește LED-ul dreapta
  lcd.clear();
}
