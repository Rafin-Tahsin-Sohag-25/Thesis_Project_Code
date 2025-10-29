#define BLYNK_TEMPLATE_ID "TMPL6bvvmUxIm"
#define BLYNK_TEMPLATE_NAME "IoT Car Safety System"
//char auth[] = "QvsqOzLYXeBWJ5h3FRYuUtnxUS-frXDr";

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<bits/stdc++.h>

// Blynk Authentication Token
char auth[] = "QvsqOzLYXeBWJ5h3FRYuUtnxUS-frXDr";

// WiFi credentials
char ssid[] = "Mawlana Mess";
char pass[] = "mawlana1234";

// Sensor Pins
#define ALCOHOL_SENSOR A0
#define EYE_BLINK_SENSOR D4
#define BUZZER D7
#define ALCOHOL_RELAY D5
#define SLEEP_RELAY D8

// Alcohol Threshold (modifiable via Blynk)
int alcoholThreshold = 400;
int sleepCounter = 0;

BlynkTimer timer;

void checkSensors() 
{
    int alcoholValue = analogRead(ALCOHOL_SENSOR);
    int eyeBlinkValue = digitalRead(EYE_BLINK_SENSOR);
    
    Blynk.virtualWrite(V1, alcoholValue);
    Blynk.virtualWrite(V2, eyeBlinkValue);
    
    if (alcoholValue > alcoholThreshold) 
    {
        digitalWrite(BUZZER, HIGH);
        digitalWrite(ALCOHOL_RELAY, LOW); // Stop the car immediately due to alcohol detection
        Blynk.logEvent("alcohol_alert", "ALERT: Alcohol detected! Car stopped.");
        delay(5000);
    } 
    else 
    {
        digitalWrite(ALCOHOL_RELAY, HIGH); // Allow car to move if no alcohol detected
    }
    
    if (eyeBlinkValue == 0) 
    { // Assuming 0 means eyes closed
        sleepCounter++;
        if (sleepCounter >= 5) 
        { // After 5 seconds
            digitalWrite(BUZZER, HIGH);
            digitalWrite(SLEEP_RELAY, LOW); // Stop the car due to sleep detection
            Blynk.logEvent("sleep_alert", "ALERT: Driver sleeping! Car stopped.");
            delay(5000);
            sleepCounter = 0; // Reset counter
        }
    } 
    else 
    {
        digitalWrite(SLEEP_RELAY, HIGH); // Allow car to move if driver is awake
        sleepCounter = 0; // Reset counter when driver is awake
    }
}

BLYNK_WRITE(V3) 
{ 
    alcoholThreshold = param.asInt(); // Update threshold from Blynk
    Blynk.virtualWrite(V4, alcoholThreshold); // Display updated threshold
}

void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    pinMode(ALCOHOL_SENSOR, INPUT);
    pinMode(EYE_BLINK_SENSOR, INPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(ALCOHOL_RELAY, OUTPUT);
    pinMode(SLEEP_RELAY, OUTPUT);
    digitalWrite(ALCOHOL_RELAY, HIGH);
    digitalWrite(SLEEP_RELAY, HIGH);
    timer.setInterval(1000L, checkSensors);
}

void loop() 
{
    Blynk.run();
    timer.run();
}
