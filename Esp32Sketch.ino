\
#define BLYNK_TEMPLATE_ID "TMPL3G1ZQEEgo"
#define BLYNK_TEMPLATE_NAME "PowerTrack"
#define BLYNK_AUTH_TOKEN "if3VKtvv6Qh0GP9d_d_IpENvbcQB90TN"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";
char auth[] = "if3VKtvv6Qh0GP9d_d_IpENvbcQB90TN";

double units = 0;
double price = 0;


const int ledPin = 27;      // Pin connected to the LED on ESP32
const int ldrPin = 2;       // Pin connected to the LDR

const int ldrThreshold = 1500;  // LDR value threshold for detecting dark to bright transition
const int ldrHighThreshold = 2000;  // LDR value threshold for considering it as HIGH state
const int blinkDelay = 500;  // Adjust the delay based on your requirements for the loop
const int debounceDelay = 50;  // Debounce delay to prevent false triggers

int ledState = LOW;         // Variable to store the current state of the LED
int lastLdrState = LOW;      // Variable to store the previous state of the LDR
int lastDebounceTime = 0;    // Variable to store the last time the input was debounced
int blinkCount = 0;         // Variable to store the count of LED blinks

BlynkTimer timer;

// This function sends LED blink count to Blynk app
void sendUnits()
{
  Blynk.virtualWrite(V0, units);
  Blynk.virtualWrite(V1, price);
  Blynk.virtualWrite(V2, units);
}


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  Serial.begin(115200);


  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendUnits);

}

void loop() {

  Blynk.run();
  timer.run();

  int ldrValue = analogRead(ldrPin);

  // Check if LDR state has changed (detects a transition from dark to bright)
  if (ldrValue > ldrThreshold && lastLdrState == LOW) {
    // Record the time when the state change occurred
    lastDebounceTime = millis();
  }

  // Check if enough time has passed since the last state change to debounce
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Toggle the LED state
    ledState = !ledState;
    digitalWrite(ledPin, ledState);

    if (ledState == HIGH) {
      blinkCount++;
      Serial.print("LED Blink Count: ");
      Serial.println(blinkCount);
    }
  }

  // Update the LDR state
  lastLdrState = (ldrValue > ldrHighThreshold) ? HIGH : LOW;

  delay(blinkDelay);

  units = blinkCount/10;
  //prices depends on area and electricity provider
  if(units<=50){
    price=units*1.95;
  }
  else if(units<=100){
    price=units*3.1;
  }
  else if(units<=200){
    price=units* 4.8;
  }
  else{
    price =units*5.10;
  }
  //and so on and the prices may vary with increase in consumption


}
