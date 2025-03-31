// Define pins for sensors and buzzer
#define pirPin 4      // PIR sensor output pin
#define buzzerPin 2   // Passive buzzer pin
#define trigPin 9     // Ultrasonic sensor trigger pin
#define echoPin 8     // Ultrasonic sensor echo pin

void setup() {
  pinMode(pirPin, INPUT);    // Set PIR sensor as input
  pinMode(buzzerPin, OUTPUT); // Set buzzer as output
  pinMode(trigPin, OUTPUT);   // Set ultrasonic trigger as output
  pinMode(echoPin, INPUT);    // Set ultrasonic echo as input
  Serial.begin(9600);        // Start serial monitor
}

void loop() {
  int motion = detectStableMotion(); // Get stable PIR reading
  int distance = getFilteredDistance(); // Get filtered ultrasonic sensor reading

  // Print the measured distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Trigger the alarm only if BOTH motion is detected AND an object is within 200 cm
  if (motion == HIGH && (distance > 0 && distance <= 200)) {  
    Serial.println("🚨 Motion & Obstacle Detected! Alarm ON!");
    playIntruderAlarm();  // Call the intruder alarm function
  } else {  
    Serial.println("ℹ️ No Threat. Alarm OFF.");
    noTone(buzzerPin);  // Turn OFF buzzer immediately
  }
  delay(500); // Small delay to reduce sensor noise
}

// Function to stabilize PIR sensor readings
int detectStableMotion() {
  int motion1 = digitalRead(pirPin);
  delay(100);
  int motion2 = digitalRead(pirPin);
  return (motion1 == HIGH && motion2 == HIGH) ? HIGH : LOW;
}

// Function to measure and filter distance using the ultrasonic sensor
int getFilteredDistance() {
  int totalDistance = 0;
  int validReadings = 0;

  for (int i = 0; i < 5; i++) { // Take 5 readings and average
    int distance = getDistance();
    if (distance > 0 && distance <= 300) { // Valid range
      totalDistance += distance;
      validReadings++;
    }
    delay(50); // Short delay to stabilize readings
  }
  
  return (validReadings > 0) ? (totalDistance / validReadings) : 999; // Return averaged value or 999 if no valid readings
}

// Function to measure distance using the ultrasonic sensor
int getDistance() {
  digitalWrite(trigPin, LOW); // Ensure trigger pin is low before sending pulse
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Send 10us pulse to trigger pin
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout to prevent infinite loop
  if (duration == 0) return 999; // If no pulse received, return out of range
  
  int distance = duration * 0.034 / 2;  // Convert duration to cm
  if (distance > 300 || distance <= 0) return 999; // Out of range filtering
  return distance;
}

// Function to play an intruder alarm sound (urgent & oscillating)
void playIntruderAlarm() {
  unsigned long startTime = millis();  // Track how long the alarm has been going
  while (millis() - startTime < 5000) {  // Sound alarm for 5 seconds max
    tone(buzzerPin, 1000); // Play first tone
    delay(250); // Short delay
    tone(buzzerPin, 1500); // Play second tone
    delay(250); // Short delay
  }
  noTone(buzzerPin); // Stop buzzer after alarm duration
}
