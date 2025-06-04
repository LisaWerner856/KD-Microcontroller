const int greenled = 2;
const int redled = 4;
const int blueled = 6;
const int yellowled = 8;

const int greenbutton = 3;
const int redbutton = 5;
const int bluebutton = 7;
const int yellowbutton = 9;

const int piezopin = 10;

int sequence[] = { 2, 2, 3 };
long randNumber;

bool buttonState = LOW;

void setup() {
  // led pins
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(yellowled, OUTPUT);
  // button pins
  pinMode(greenbutton, INPUT);
  pinMode(redbutton, INPUT);
  pinMode(bluebutton, INPUT);
  pinMode(yellowbutton, INPUT);

  // piezo pin
  pinMode(piezopin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Generate a tone of 1000Hz for 1 second
  tone(piezopin, 440);
  delay(500);
  // Stop the tone
  noTone(piezopin);
  delay(500);  // Wait for 1 second before repeating

  tone(piezopin, 1000);
  delay(500);

  noTone(piezopin);
  delay(500);

  blinkLed(greenled);
  blinkLed(redled);
  blinkLed(blueled);
  blinkLed(yellowled);

  waitForButtonPress(greenbutton);
  waitForButtonPress(redbutton);
  waitForButtonPress(bluebutton);
  waitForButtonPress(yellowbutton);

  Serial.println("Checks completed!");
}

void blinkLed(int ledpin) {
  digitalWrite(ledpin, HIGH);
  delay(1000);
  digitalWrite(ledpin, LOW);
}

void waitForButtonPress(int buttonPin) {
  Serial.println("Checking button...");
  bool buttonPressed = digitalRead(buttonPin);
  Serial.println(buttonPressed);
  while (buttonPressed == LOW) {
    Serial.println("No button detected yet...");
    buttonPressed = digitalRead(buttonPin);
    Serial.println(buttonPressed);
    Serial.println(digitalRead(buttonPin));
  }
  Serial.println("Sucess~");
}