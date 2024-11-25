// Pin definitions
const int buttonPin = 7;  // Pin connected to the pushbutton
const int relayPin = 4;   // Pin connected to the relay module

// Variables for button and relay state
bool relayState = false;  // Tracks the state of the relay (ON/OFF)
bool lastButtonState = LOW;  // Previous state of the button
bool currentButtonState = LOW;  // Current state of the button

// Debounce variables
unsigned long lastDebounceTime = 0;  // Stores the last time the button was toggled
const unsigned long debounceDelay = 50;  // Debounce delay in milliseconds

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Set the button pin as input with internal pull-up
  pinMode(relayPin, OUTPUT);         // Set the relay pin as output
  digitalWrite(relayPin, LOW);       // Ensure the relay is OFF initially
  Serial.begin(9600);                // Initialize serial communication (optional)
}

void loop() {
  // Read the state of the button
  int reading = digitalRead(buttonPin);

  // Check if the button state has changed (pressed or released)
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // Update the debounce timer
  }

  // If enough time has passed since the last toggle
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button state has changed and is now pressed
    if (reading == LOW && currentButtonState == HIGH) {
      relayState = !relayState;  // Toggle the relay state
      digitalWrite(relayPin, relayState ? HIGH : LOW);  // Update the relay
      Serial.println(relayState ? "Relay ON" : "Relay OFF");  // Debug message
    }
    currentButtonState = reading;  // Update the current button state
  }

  // Save the button state for the next loop
  lastButtonState = reading;

  

  // Other non-blocking code can go here
}
