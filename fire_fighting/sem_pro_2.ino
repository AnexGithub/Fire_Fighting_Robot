#include <Servo.h>

// Pin Definitions
const int smokeSensorPin = A3;      // MQ-6 smoke sensor
const int flameSensorPin1 = A0;     // Flame sensor 1
const int flameSensorPin2 = A1;     // Flame sensor 2
const int flameSensorPin3 = A2;     // Flame sensor 3
const int RELAY_PIN1 = 7;           // Relay for water pump (channel 1)
const int RELAY_PIN2 = 8;           // Relay for smoke sucker motor (channel 2)
const int servoPin = 9;             // Servo motor connected to pin 9

Servo waterServo;                   // Create a Servo object

// Smoke sensor settings
float smokeSensorValue;
int smokeThreshold = 50;            // Adjusted threshold value for smoke detection

void setup() {
  pinMode(RELAY_PIN1, OUTPUT);       // Set RELAY_PIN1 as output for water pump
  pinMode(RELAY_PIN2, OUTPUT);       // Set RELAY_PIN2 as output for smoke sucker motor
  pinMode(smokeSensorPin, INPUT);   // Set smoke sensor pin as input
  pinMode(flameSensorPin1, INPUT);  // Set flame sensor pins as input
  pinMode(flameSensorPin2, INPUT);
  pinMode(flameSensorPin3, INPUT);

  waterServo.attach(servoPin);      // Attach the servo to the defined pin

  Serial.begin(9600);               // Initialize serial communication
  Serial.println("System initialized. Monitoring sensors...");
  
  digitalWrite(RELAY_PIN1, HIGH);   // Ensure water pump is initially OFF (active LOW)
  digitalWrite(RELAY_PIN2, HIGH);   // Ensure smoke sucker motor is initially OFF (active LOW)
  waterServo.write(0);              // Initialize the servo at 0 degrees

  delay(2000);                      // Allow the MQ-6 to warm up
}

void loop() {
  // Read the smoke sensor value
  smokeSensorValue = analogRead(smokeSensorPin); 
  Serial.print("Smoke Sensor Value: ");
  Serial.print(smokeSensorValue);

  if (smokeSensorValue > smokeThreshold) { // Smoke detection logic
    Serial.println(" | Smoke detected!");
    digitalWrite(RELAY_PIN2, LOW);    // Turn ON smoke sucker motor (active LOW)
  } else {
    Serial.println(" | No smoke detected!");
    digitalWrite(RELAY_PIN2, HIGH);   // Turn OFF smoke sucker motor (active LOW)
  }

  // Read values from flame sensors
  int flame1 = digitalRead(flameSensorPin1);  
  int flame2 = digitalRead(flameSensorPin2);  
  int flame3 = digitalRead(flameSensorPin3);  

  // Display flame sensor values for debugging
  Serial.print("Flame1: ");
  Serial.print(flame1);
  Serial.print(" | Flame2: ");
  Serial.print(flame2);
  Serial.print(" | Flame3: ");
  Serial.println(flame3);

  // Check if any flame sensor detects fire
  if (flame1 == LOW || flame2 == LOW || flame3 == LOW) {
    Serial.println("Fire detected! Activating water pump and servo...");
    digitalWrite(RELAY_PIN1, LOW);  // Turn ON the water pump (active LOW)
    sprayWater();
  } else {
    Serial.println("No fire detected.");
    digitalWrite(RELAY_PIN1, HIGH); // Ensure the water pump is OFF
  }

  delay(1000); // Wait before the next reading
}

// Function to control the servo to spray water
void sprayWater() {
  // Sweep the servo from 0 to 180 degrees (spraying water)
  for (int pos = 0; pos <= 180; pos++) {
    waterServo.write(pos);  // Set servo position
    delay(15);               // Wait for the servo to reach the position
  }

  // Sweep the servo back from 180 to 0 degrees (reset position)
  for (int pos = 180; pos >= 0; pos--) {
    waterServo.write(pos);  // Set servo position
    delay(15);               // Wait for the servo to reach the position
  }

  // Turn OFF the water pump (active LOW relay)
  digitalWrite(RELAY_PIN1, HIGH);
}
