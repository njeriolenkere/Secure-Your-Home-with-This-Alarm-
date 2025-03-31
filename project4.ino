#define pirPin 4      // PIR sensor output pin
#define buzzerPin 2   // Passive buzzer pin

void setup() {
  pinMode(pirPin, INPUT);    // PIR sensor as input
  pinMode(buzzerPin, OUTPUT); // Buzzer as output
  Serial.begin(9600);        // Start serial monitor
}

void loop() {
  int motion = digitalRead(pirPin); // Read PIR sensor state

  if (motion == HIGH) {  // Motion detected
    Serial.println("🚨 Motion Detected! Alarm ON! 🚨");
    playIntruderAlarm();  // Call the intruder alarm function
  } else {  
    Serial.println("❌ No Motion. Alarm OFF.");
    noTone(buzzerPin);  // Turn OFF buzzer immediately
  }
}

// Function to play an intruder alarm sound (urgent & oscillating)
void playIntruderAlarm() {
  unsigned long startTime = millis();  // Track how long the alarm has been going
  while (digitalRead(pirPin) == HIGH) {  // Keep running only if motion is detected
    unsigned long elapsedTime = millis() - startTime;  // Elapsed time since the alarm started

    // Alternate between two frequencies for an urgent sound
    if (elapsedTime % 500 < 250) {  
      tone(buzzerPin, 1000);  // Play 1000 Hz
    } else {
      tone(buzzerPin, 1500);  // Play 1500 Hz
    }

    // Check if motion is gone and stop the alarm immediately
    if (digitalRead(pirPin) == LOW) {  
      noTone(buzzerPin);
      return;  // Stop siren if motion stops
    }
  }
}
