#include "RudderPedal.h"
#include <EEPROM.h>

RudderPedal::RudderPedal(int rudderPin, int leftBrakePin, int rightBrakePin)
  : _rudder(rudderPin),
    _leftBrake(leftBrakePin),
    _rightBrake(rightBrakePin),
    _joystick(JOYSTICK_DEFAULT_REPORT_ID,
              JOYSTICK_TYPE_JOYSTICK, 0, 0,
              false, false, false, true, true, false,
              true, false, false, false, false) {}

RudderPedal::~RudderPedal() {}

void RudderPedal::Init() {
  cal_values_t values;
  EEPROM.get(0, values);

  _rudder.SetMinMax(values.rudder);
  _leftBrake.SetMinMax(values.leftBrake);
  _rightBrake.SetMinMax(values.rightBrake);

  this->UpdateJoystickRange(values);
  _joystick.begin();
}

void RudderPedal::UpdateControls() {
  int rudder, left, right;
  rudder = _rudder.ReadFilter(FILTER_COUNT);
  left = _leftBrake.ReadFilter(FILTER_COUNT);
  right = _rightBrake.ReadFilter(FILTER_COUNT);
  _joystick.setRudder(rudder);
  _joystick.setRxAxis(left);
  _joystick.setRyAxis(right);

  Serial.println(String(rudder) + "|" + String(left) + "|" + String(right));
}

void RudderPedal::StartCal() {
  min_max_t initValues = { .min = Control::DEFAULT_MIN_MAX, .max = Control::DEFAULT_MIN_MAX };
  _rudder.SetMinMax(initValues);
  _leftBrake.SetMinMax(initValues);
  _rightBrake.SetMinMax(initValues);
}

void RudderPedal::Calibrate() {
  cal_values_t values;
  values.rudder = _rudder.Calibrate();
  values.leftBrake = _leftBrake.Calibrate();
  values.rightBrake = _rightBrake.Calibrate();
  this->UpdateJoystickRange(values);
}

void RudderPedal::WriteCalValues() {
  cal_values_t values;
  values.rudder = _rudder.GetMinMax();
  values.leftBrake = _leftBrake.GetMinMax();
  values.rightBrake = _rightBrake.GetMinMax();
  EEPROM.put(0, values);
}

void RudderPedal::UpdateJoystickRange(const cal_values_t values) {
  _joystick.setRudderRange(values.rudder.min, values.rudder.max);
  _joystick.setRxAxisRange(values.leftBrake.min, values.leftBrake.max);
  _joystick.setRyAxisRange(values.rightBrake.min, values.rightBrake.max);

  PrintMinMax();
}

void RudderPedal::PrintMinMax() {
  cal_values_t values;
  values.rudder = _rudder.GetMinMax();
  values.leftBrake = _leftBrake.GetMinMax();
  values.rightBrake = _rightBrake.GetMinMax();
  Serial.println(String("Min: ") + String(values.rudder.min) + "|" + String(values.leftBrake.min) + "|" + String(values.rightBrake.min));
  Serial.println(String("Max: ") + String(values.rudder.max) + "|" + String(values.leftBrake.max) + "|" + String(values.rightBrake.max));
}
