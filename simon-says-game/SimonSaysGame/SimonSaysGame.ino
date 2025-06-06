#include "pitches.h"

const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5 };

const int arrayLenght = 4;
// --- Pin setup --- //
// 2 = green, 4 = red, 6 = blue, 8 = yellow
const int ledArray[arrayLenght] = { 2, 4, 6, 8 };
// 3 = green button, 5 = red button, 7 = blue button, 9 = yellow button
const int buttonArray[arrayLenght] = { 3, 5, 7, 9 };
const int piezoPin = 10;
// --- --- --- --- --- //

long randNumber;

// const int gameLength = 100;
const int gameLength = 5;
int gameSequence[gameLength] = { 0 };
int gameRound = 0;
bool roundCompleted = false;

bool lastButtonState = HIGH;
bool buttonState = LOW;


void setup() {
  for (int i = 0; i < arrayLenght; i++) {
    // led pins
    pinMode(ledArray[i], OUTPUT);
    // button pins
    pinMode(buttonArray[i], INPUT_PULLUP);
  }
  // piezo pin
  pinMode(piezoPin, OUTPUT);

  Serial.begin(9600);

  // Seed the random
  // Make sure nothing is plugged into the A3 pin!!!
  randomSeed(analogRead(A3));
}

void levelUpSound() {
  tone(piezoPin, NOTE_E4);
  delay(150);
  tone(piezoPin, NOTE_G4);
  delay(150);
  tone(piezoPin, NOTE_E5);
  delay(150);
  tone(piezoPin, NOTE_C5);
  delay(150);
  tone(piezoPin, NOTE_D5);
  delay(150);
  tone(piezoPin, NOTE_G5);
  delay(150);
  noTone(piezoPin);
}
void gameOverSound() {
  // Play a Wah-Wah-Wah-Wah sound
  tone(piezoPin, NOTE_DS5);
  delay(300);
  tone(piezoPin, NOTE_D5);
  delay(300);
  tone(piezoPin, NOTE_CS5);
  delay(300);

  noTone(piezoPin);
}


void loop() {
  // if the game round is smaller than the game lenght (max rounds) and the roundCompleted is false.
  if (gameRound < gameLength && !roundCompleted) {
    // new round
    // add random sequence to the game
    gameSequence[gameRound] = random(0, 4);

    gameRound++;
    playSequence();

    roundCompleted = true;

    delay(1000);
  } 
  roundCompleted = checkPlayerInput();
}
void blinkLights(){
  digitalWrite(ledArray[0], HIGH);
  digitalWrite(ledArray[1], HIGH);
  digitalWrite(ledArray[2], HIGH);
  digitalWrite(ledArray[3], HIGH);
  delay(100);
  digitalWrite(ledArray[0], LOW);
  digitalWrite(ledArray[1], LOW);
  digitalWrite(ledArray[2], LOW);
  digitalWrite(ledArray[3], LOW);
  delay(500);
}
void playSequence() {
  // bleep bloop game things
  // Serial.print("Sequence: ");
  for (int i = 0; i < gameRound; i++) {
    // Serial.print(gameSequence[i]);
    // Serial.print(", ");
    // light up light and play sound
    digitalWrite(ledArray[gameSequence[i]], HIGH);
    tone(piezoPin, gameTones[gameSequence[i]]);
    delay(500);
    digitalWrite(ledArray[gameSequence[i]], LOW);
    noTone(piezoPin);
    delay(500);
  }
}

int playerInput() {
  // loop over all the buttons and see if any of them are pressed
  for (int i = 0; i < arrayLenght; i++) {
    if (digitalRead(buttonArray[i]) == LOW) {
      Serial.print("Pressed: ");
      lastButtonState == LOW;
      Serial.println(i);
      return i;
    }
  }
  // return value for if no button was pressed
  return -1;
}

bool checkPlayerInput() {
  Serial.print("Game round: ");
  Serial.println(gameRound);
  for (int i = 0; i < gameRound; i++) {
    int expectedButton = gameSequence[i];
    int actualButtonPressed = playerInput();
    while(actualButtonPressed == -1){
      actualButtonPressed = playerInput();
    }
    if (actualButtonPressed != expectedButton && lastButtonState == LOW) {
      Serial.println("Wrong");
      gameOverSound();
      blinkLights();
      blinkLights();
      blinkLights();
      lastButtonState == HIGH;
      return false;
    }
    else {
      Serial.println("Correct!");
      lastButtonState == HIGH;
      return true;
    }
  }
}