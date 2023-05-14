#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "sequence-page.h"
#include "result-page.h"

const char* ssid = "";
const char* password = "";

const int redDiodePin = D4;
bool redDiodeStatus = LOW;
const int greenDiodePin = D1;
bool greenDiodeStatus = LOW;
const int blueDiodePin = D2;
bool blueDiodeStatus = LOW;

const int redButtonPin = D8;
const int greenButtonPin = D6;
const int blueButtonPin = D7;

String initialSequence = "";
String checkSequence = "";
int pressCounter = 0;
int steps = 8;

ESP8266WebServer server(80);

void handle_root() {
    delay(500);
    server.send(200, "text/html", sequencePage);
}

void setup() {
    Serial.begin(115200);

    pinMode(redDiodePin, OUTPUT);
    pinMode(greenDiodePin, OUTPUT);
    pinMode(blueDiodePin, OUTPUT);
    pinMode(redButtonPin, INPUT_PULLUP);
    pinMode(greenButtonPin, INPUT_PULLUP);
    pinMode(blueButtonPin, INPUT_PULLUP);

    Serial.println("Connecting to ");
    Serial.println(ssid);
  
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    
    Serial.print("IP: ");  
    Serial.println(WiFi.localIP());

    server.on("/start", handle_root);
    server.on("/showSequence", handle_showSequence);
    server.on("/progress", handle_showProgress);
    server.on("/getCurrentState", handle_getCurrentState);

    server.begin();

    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();

    int redButtonState = digitalRead(redButtonPin);
    int greenButtonState = digitalRead(greenButtonPin);  
    int blueButtonState = digitalRead(blueButtonPin);

    if (redButtonState == HIGH) {
        checkSequence = checkSequence + '1';
        pressCounter += 1;
        blink(redDiodePin, 100);
    }
    if (greenButtonState == HIGH) {
        checkSequence = checkSequence + '2';
        pressCounter += 1;
        blink(greenDiodePin, 100);
    }
    if (blueButtonState == HIGH) {
        checkSequence = checkSequence + '3';
        pressCounter += 1;
        blink(blueDiodePin, 100);
    }

    if (pressCounter > steps) {
        pressCounter = 0;
        initialSequence = "";
        checkSequence = "";
    }
}

void handle_showSequence() {
    delay(1000);
    pressCounter = 0;
    checkSequence = "";
    
    if (server.arg("sequence") != "") {
        initialSequence = server.arg("sequence");
        steps = initialSequence.length();
    }

    if (initialSequence != "") {
        for (int i = 0; i < steps; ++i) {
            if (initialSequence[i] == '1') {
                blink(redDiodePin, 1000);
            }
            if (initialSequence[i] == '2') {
                blink(greenDiodePin, 1000);
            }
            if (initialSequence[i] == '3') {
                blink(blueDiodePin, 1000);
            }
        }
    }
}

void handle_showProgress() {
    server.send(200, "text/html", resultPage);
}

void handle_getCurrentState() {
    server.send(200, "text/plain", String(steps) + " " + initialSequence + " " +  checkSequence);
}

void blink(int pin, int time) {
    digitalWrite(pin, HIGH);
    delay(time);
    digitalWrite(pin, LOW);
    delay(time);
}
