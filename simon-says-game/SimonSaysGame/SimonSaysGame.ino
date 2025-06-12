#include "pitches.h"

const int arrayLength = 4;
// --- Pin setup --- //
// 2 = green, 4 = red, 6 = blue, 8 = yellow
const int ledArray[arrayLength] = { 2, 4, 6, 8 };
// 3 = green button, 5 = red button, 7 = blue button, 9 = yellow button
const int buttonArray[arrayLength] = { 3, 5, 7, 9 };
const int piezoPin = 10;
// --- --- --- --- --- //

const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5 };
long randNumber;

// const int gameLength = 100;
const int gameLength = 5;
int gameSequence[gameLength] = { 0 };
int gameRound = 0;

bool roundReadyForInput = false;
bool endHandled = false;
bool gameOver = false;

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
void victorySound() {
  tone(piezoPin, NOTE_C4, 200);
  delay(200);
  tone(piezoPin, NOTE_E4, 200);
  delay(200);
  tone(piezoPin, NOTE_G4, 200);
  delay(200);
  tone(piezoPin, NOTE_C5, 300);  // jump octave
  delay(300);

  tone(piezoPin, NOTE_G4, 150);
  delay(150);
  tone(piezoPin, NOTE_B4, 150);
  delay(150);
  tone(piezoPin, NOTE_D5, 400);
  delay(400);

  noTone(piezoPin);
}

void setup() {
  for (int i = 0; i < arrayLength; i++) {
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

  blinkLights();
  blinkLights();
  blinkLights();
  // Start game sound
  tone(piezoPin, NOTE_G4, 150);
  delay(150);
  tone(piezoPin, NOTE_A4, 150);
  delay(150);
  tone(piezoPin, NOTE_B4, 150);
  delay(150);
  tone(piezoPin, NOTE_E5, 300);  // bright jump!
  delay(300);

  tone(piezoPin, NOTE_D5, 200);
  delay(200);
  tone(piezoPin, NOTE_G5, 400);  // solid finish
  delay(400);

  noTone(piezoPin);
  // Little delay to seperate startup from first beep
  delay(500);
}

void loop() {
  if (!gameOver && gameRound < gameLength) {

    // if the game round is smaller than the game lenght (max rounds) and the roundCompleted is false.
    if (gameRound < gameLength && !roundReadyForInput) {
      // new round
      // add random sequence to the game
      gameSequence[gameRound] = random(0, arrayLength);

      gameRound++;
      playSequence();

      roundReadyForInput = true;

      delay(1000);
    }
    bool result = checkPlayerInput();
    roundReadyForInput = result;
    gameOver = result;
  }
  if (gameOver && !endHandled) {
    endHandled = true;
    gameOverSound();
    blinkLights();
    blinkLights();
    blinkLights();
    delay(100);
  }

  if (gameRound >= gameLength && !endHandled) {
    endHandled = true;
    Serial.println("You win!");
    victorySound();
    blinkLights();
    blinkLights();
    blinkLights();
  }
}

void blinkLights() {
  digitalWrite(ledArray[0], HIGH);
  digitalWrite(ledArray[1], HIGH);
  digitalWrite(ledArray[2], HIGH);
  digitalWrite(ledArray[3], HIGH);
  delay(100);
  digitalWrite(ledArray[0], LOW);
  digitalWrite(ledArray[1], LOW);
  digitalWrite(ledArray[2], LOW);
  digitalWrite(ledArray[3], LOW);
  delay(100);
}
// power led's  and play sound
void playSequence() {
  for (int i = 0; i < gameRound; i++) {
    digitalWrite(ledArray[gameSequence[i]], HIGH);
    tone(piezoPin, gameTones[gameSequence[i]]);
    delay(500);
    digitalWrite(ledArray[gameSequence[i]], LOW);
    noTone(piezoPin);
    delay(100);
  }
}

// returns which button the player pressed, only detects button down press
int playerInput() {
  for (int i = 0; i < arrayLength; i++) {
    if (digitalRead(buttonArray[i]) == LOW) {
      delay(10);  // debounce delay

      // Check again after delay to confirm it's still pressed
      if (digitalRead(buttonArray[i]) == LOW) {
        digitalWrite(ledArray[i], HIGH);
        tone(piezoPin, gameTones[i]);
        // Wait for release
        while (digitalRead(buttonArray[i]) == LOW) {
          // do nothing, just wait
        }
        digitalWrite(ledArray[i], LOW);
        noTone(piezoPin);

        Serial.print("Pressed: ");
        Serial.println(i);
        return i;
      }
    }
  }

  return -1;  // no valid press
}

// checks if the player input corresponds to the expected input
bool checkPlayerInput() {
  Serial.println("");
  Serial.print("Round");
  Serial.println(gameRound);
  for (int i = 0; i < gameRound; i++) {
    int input = playerInput();
    while (input == -1) {
      input = playerInput();
    }
    if (input != gameSequence[i]) {
      Serial.println("Game over!");
      return true;
    }
  }
  if (gameRound >= gameLength) {
    return false;
  }
  delay(200);
  blinkLights();
  levelUpSound();
  delay(100);
  return false;
}