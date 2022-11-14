#include <ezBuzzer.h>
#include <SerialLCD.h>
#include <SoftwareSerial.h>
#include <math.h>

const int LED_PIN = 1;
const int BUTTON_PIN = 2;
const int BUZZER_PIN = 3;
const int ROTARY_PIN = A0;

const int ADC_REF = 5;
const int GROVE_VCC = 5;
const int FULL_ANGLE = 25;

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWZYZ";
char solution[] = "ALUMAR";
char attempt[] = "______";
bool checked = false;

ezBuzzer buzzer(BUZZER_PIN);
SerialLCD slcd(11, 12);

int lightState = LOW;
int lastButtonState;
int currentButtonState;

int currentLcdColumn = 0;


int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int noteDurations[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ROTARY_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);

  currentButtonState = digitalRead(BUTTON_PIN);

  slcd.begin();
  slcd.backlight(); 
  slcd.print("______");
  slcd.setCursor(0, 1);
  slcd.print(".");

}

void loop() {
  buzzer.loop();


  if (strcmp(attempt, solution) == 0 && !checked) {
    checked = true;
    digitalWrite(LED_PIN, HIGH);
    

    slcd.clear();
    delay(100);
    slcd.print(" CODIGO -> 1111");

    slcd.setCursor(1, 1);
    slcd.print(" PARABENS! :)");

    for (int i = 0; i < 3; i++) {
      slcd.noBacklight();
      digitalWrite(LED_PIN, LOW);
      delay(500);
      slcd.backlight();
      digitalWrite(LED_PIN, HIGH);
      delay(500);
    }

    if (buzzer.getState() == BUZZER_IDLE) {
      int length = sizeof(noteDurations) / sizeof(int);
      buzzer.playMelody(melody, noteDurations, length); // playing
    }
    buzzer.loop();

  }

  if (!checked) {
    int sensorValue = analogRead(ROTARY_PIN);
    float voltage = (float)sensorValue * ADC_REF / 1023;
    int letterIndex = round((voltage * FULL_ANGLE) / GROVE_VCC);
    slcd.setCursor(currentLcdColumn, 0);
    slcd.print(alphabet[letterIndex]);

    attempt[currentLcdColumn] = alphabet[letterIndex];

    lastButtonState = currentButtonState;
    currentButtonState = digitalRead(BUTTON_PIN);

    if (lastButtonState == HIGH && currentButtonState == LOW) {
      slcd.setCursor(currentLcdColumn, 1);
      slcd.print(" ");
      currentLcdColumn = (currentLcdColumn + (currentLcdColumn == 1 ? 1 : 1)) % 6;
      slcd.setCursor(currentLcdColumn, 1);
      slcd.print(".");
      buzzer.beep(50);
    }
  }

}