#include <NewPing.h>  // Include the NewPing library for ultrasonic sensor

#define pirPin 4      // PIR sensor output pin
#define buzzerPin 2   // Passive buzzer pin
#define trigPin 9     // Ultrasonic sensor trigger pin (digital pin 9)
#define echoPin 8     // Ultrasonic sensor echo pin (digital pin 8)
#define MAX_DISTANCE 300 // Maximum distance to check (in cm)
#define TRIGGER_DISTANCE 200 // Distance to trigger the alarm (in cm)

NewPing sonar(trigPin, echoPin, MAX_DISTANCE); // Create NewPing object

void setup() {
  pinMode(pirPin, INPUT);    // PIR sensor as input
  pinMode(buzzerPin, OUTPUT); // Buzzer as output
  Serial.begin(9600);        // Start serial monitor
}

void loop() {
  int motion = digitalRead(pirPin); // Read PIR sensor state
  delay(50); // Small delay for stability

  // Measure distance
  unsigned int distance = sonar.ping_cm(); // Get distance in cm
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (motion == HIGH) {  // Motion detected
    // Check if the distance is less than or equal to 200 cm to trigger the alarm
    if (distance > 0 && distance <= TRIGGER_DISTANCE) { // Ensure distance is valid
      Serial.println("🚨 Motion Detected! Alarm ON! 🚨");
      playIntruderAlarm();  // Call the intruder alarm function
    } else {
      Serial.println("❌ No Intruder Detected (Distance too far).");
      digitalWrite(buzzerPin, LOW);  // Turn OFF buzzer immediately
    }
  } else {  
    Serial.println("❌ No Motion. Alarm OFF.");
    digitalWrite(buzzerPin, LOW);  // Turn OFF buzzer immediately
  }

  delay(100); // Add a delay to prevent overwhelming the sensor
}

// Function to play an intruder alarm sound (urgent & oscillating)
void playIntruderAlarm() {
  unsigned long startTime = millis();  // Track how long the alarm has been going
  while (digitalRead(pirPin) == HIGH) {  // Keep running only if motion is detected
    unsigned long elapsedTime = millis() - startTime;  // Elapsed time since the alarm started

    // Alternate between two frequencies for an urgent sound
    if (elapsedTime % 500 < 250) {  
      playTone(1000, 100);  // Play 1000 Hz for 100 ms
    } else {
      playTone(1500, 100);  // Play 1500 Hz for 100 ms
    }

    // Check if motion is gone and stop the alarm immediately
    if (digitalRead(pirPin) == LOW) {  
      digitalWrite(buzzerPin, LOW);
      return;  // Stop siren if motion stops
    }
  }
}

// Function to generate a tone using software
void playTone(int frequency, int duration) {
  long delayValue = 1000000 / frequency / 2; // Calculate the delay value
  long numCycles = frequency * duration / 1000; // Calculate the number of cycles

  for (long i = 0; i < numCycles; i++) {
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delayMicroseconds(delayValue);   // Wait for half the period
    digitalWrite(buzzerPin, LOW);   // Turn off the buzzer
    delayMicroseconds(delayValue);   // Wait for half the period
  }
}
