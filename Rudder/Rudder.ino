//####################################################
//      Includes
//####################################################
#include "RudderPedal.h"

//####################################################
//      Constants
//####################################################
#define RUDDER_PIN A0
#define LEFT_BRAKE_PIN A1
#define RIGHT_BRAKE_PIN A2
#define CALIBRATE_PIN 7

//####################################################
//      Types
//####################################################
typedef enum {
  NORMAL,
  CAL_START,
  CALIBRATION,
  CAL_FINISHED
} RudderPedalState_t;

//####################################################
//      Variables
//####################################################

RudderPedal rudderPedal(RUDDER_PIN, LEFT_BRAKE_PIN, RIGHT_BRAKE_PIN);
static volatile RudderPedalState_t s_state = NORMAL;

//####################################################
//      Functions
//####################################################
void setup() {
  Serial.begin(9600);
  pinMode(CALIBRATE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CALIBRATE_PIN), calibrationSwitchISR, CHANGE);

  rudderPedal.Init();
}

void loop() {
  // rudderPedal.PrintMinMax();
  switch (s_state) {
    case NORMAL:
      rudderPedal.UpdateControls();
      break;
    case CAL_START:
      rudderPedal.StartCal();
      s_state = CALIBRATION;
      break;
    case CALIBRATION:
      rudderPedal.Calibrate();
      break;
    case CAL_FINISHED:
      rudderPedal.WriteCalValues();
      s_state = NORMAL;
      break;
    default:
      break;
  }

  delay(50);
}

inline void calibrationSwitchISR() {
  int switchState = digitalRead(CALIBRATE_PIN);
  switch (s_state) {
    case NORMAL:
    case CAL_FINISHED:
      if (switchState == LOW) {
        s_state = CAL_START;
      }
      break;
    case CAL_START:
    case CALIBRATION:
      if (switchState == HIGH) {
        s_state = CAL_FINISHED;
      }
      break;
    default:
      s_state = NORMAL;
      break;
  }
}
