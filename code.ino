// Pin definitions
const int motionPin = 2;      // The Switch
const int ownerButton = 3;    // The Button
const int redLED = 6;
const int yellowLED = 5;
const int greenLED = 4;
const int buzzer = 7;

// Timer
unsigned long switchClosedTime = 0;
const unsigned long warningLimit = 5000; // 5 seconds

// States
bool ownerOverride = false;
bool timingStarted = false;

void setup() {
  pinMode(motionPin, INPUT_PULLUP);
  pinMode(ownerButton, INPUT_PULLUP);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  bool switchClosed = (digitalRead(motionPin) == LOW);
  bool buttonPressed = (digitalRead(ownerButton) == LOW);

  // 1. OWNER OVERRIDE
  if (buttonPressed) {
    ownerOverride = true;
  }

  // 2. THE START TRIGGER (Yellow comes on)
  if (switchClosed && !timingStarted && !ownerOverride) {
    switchClosedTime = millis(); // Capture the EXACT start time
    timingStarted = true;        // Lock the timer so it doesn't restart
  }

  // 3. RESET (Only happens if switch is actually opened)
  if (!switchClosed) {
    timingStarted = false;
    ownerOverride = false;
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    noTone(buzzer);
    return; 
  }

  // 4. ACTION PHASE
  if (ownerOverride) {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    noTone(buzzer);
  } 
  else if (timingStarted) {
    unsigned long elapsed = millis() - switchClosedTime;

    if (elapsed < warningLimit) {
      // YELLOW PHASE (0 - 5.0 seconds)
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, HIGH);
      digitalWrite(greenLED, LOW);
      tone(buzzer, 500); 
    } 
    else {
      // GREEN PHASE (5.0 seconds +)
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, HIGH);
      tone(buzzer, 2000); // Higher pitch for Green
    }
  }
}
