#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "sequence-page.h"
#include "result-page.h"

const char* ssid = "Keenetic-5975_5G";
const char* password = "8jdWMACE";

ESP8266WebServer server(80);

const int redDiodePin = D1;
bool redDiodeStatus = LOW;
const int greenDiodePin = D4;
bool greenDiodeStatus = LOW;
const int blueDiodePin = D3;
bool blueDiodeStatus = LOW;

const int redButtonPin = D2;
const int greenButtonPin = D5;
const int blueButtonPin = D6;

String initialSequence = "";
String checkSequence = "";
int pressCounter = 0;
int steps = 8;

void setup() {
    Serial.begin(115200);

    pinMode(redDiodePin, OUTPUT);
    pinMode(greenDiodePin, OUTPUT);
    pinMode(blueDiodePin, OUTPUT);

    Serial.println("Connecting to ");
    Serial.println(ssid);
    Serial.println("");

    WiFi.begin(ssid, password);

    server.on("/", handle_root);
    server.on("/showSequence", handle_showSequence);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();

    int redButtonState = digitalRead(redButtonPin);
    if (redButtonState == HIGH) {
        checkSequence + '1';
        pressCounter += 1;
        blink(redDiodePin);
        handle_showProgress();
    }
    int greenButtonState = digitalRead(greenButtonPin);
    if (greenButtonState == HIGH) {
        checkSequence + '2';
        pressCounter += 1;
        blink(greenDiodePin);
        handle_showProgress();
    }
    int blueButtonState = digitalRead(blueButtonPin);
    if (blueButtonState == HIGH) {
        checkSequence + '3';
        pressCounter += 1;
        blink(blueDiodePin);
        handle_showProgress();
    }

    if (pressCounter == steps) {
        pressCounter = 0;
        initialSequence = "";
        checkSequence = "";
    }
}

void handle_root() {
    String code = sequencePage;
    server.send(200, "text/html", code);
}

void handle_showSequence() {
    pressCounter = 0;
    checkSequence = "";

    if (server.arg("sequence") != "") {
        initialSequence = server.arg("sequence");
        steps = initialSequence.length();
    }

    if (initialSequence != "") {
        for (int i = 0; i < steps; ++i) {
            if (initialSequence[i] == '1') {
                blink(redDiodePin);
            }
            if (initialSequence[i] == '2') {
                blink(greenDiodePin);
            }
            if (initialSequence[i] == '3') {
                blink(blueDiodePin);
            }
        }
    }
}

void handle_showProgress() {
    String initialSequenceCode = "<span id=\"initialSequence\" class=\"hidden\">" + initialSequence + "</span>";
    String checkSequenceCode = "<span id=\"checkSequence\" class=\"hidden\">" + checkSequence + "</span>";
    String stepsAmountCode = "or (let i = 1; i <= " + steps + "; i++)"

    String code = resultPartOne
        + initialSequenceCode + resultPartTwo
        + checkSequenceCode + resultPartThree
        + stepsAmountCode + resultPartFour;

    server.send(200, "text/html", code);
}

void blink(int pin) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
}
