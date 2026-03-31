#include "UltraSoundSensor.h"
#include "Buzzer.h"
#include "FSM.h"
#include "SevenSegmentDisplay.h"
#include <vector>
#include "timer.h"
#include "WiFi.h"

using namespace std;

int ledDisplayPin[] = {23, 22, 16, 17, 18, 21, 19};
const char* SSID     = "Ynough";
const char* PASSWORD = "ynough2ynov";

enum {
  IDLE,
  DETECTED
};

FSM fsm;
UltraSoundSensor sensor;
SevenSegmentDisplay display;
Led validLed(0);
Buzzer buzzer(0);
Timer t;

void setup() {
  Serial.begin(9600);

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  //led display
  vector<Led> leds;
  for (int ledPin : ledDisplayPin) {
    leds.push_back(Led(ledPin));
  }
  display.init(leds);
  
  //sensor
  sensor.init(15, 27, 0, 20);

  //buzzer
  buzzer = Buzzer(26);
  buzzer.toggle();
  delay(50);
  buzzer.toggle();

  //validate led
  validLed = Led(25);
  validLed.toggle();
  delay(500);
  validLed.toggle();

  fsm
    .ADD_STATE(IDLE)
    .ADD_STATE(DETECTED);
  
  fsm
    .addTransition(IDLE, DETECTED, [](){
      return sensor.objectDetected();
    })
    .addTransition(DETECTED, IDLE, [](){
      return t.isExpired();
    })
    .addTransition(IDLE, IDLE, []() {
      return !sensor.objectDetected();
    });

  fsm
    .onEnter(IDLE, []() {
      display.showScore();
      sensor.resetTrig();
      sensor.setTrig();
      sensor.resetTrig();
      sensor.updateTime();
      sensor.updateDist();
    })
    .onExit(IDLE, []() {
      sensor.resetData();
    })
    .onEnter(DETECTED, []() {
      t.start(300);
      buzzer.toggle();
      display.addPoint();
    })
    .onExit(DETECTED, []() {
      t.reset();
      buzzer.toggle();
    });

  fsm.start(IDLE);
}

void loop() {
  fsm.update();
}
