#include "Arduino.h"
#include "Control.h"

Control::Control(int pin)
  : _pin(pin) {
  _minMax.min = DEFAULT_MIN_MAX;
  _minMax.max = DEFAULT_MIN_MAX;
}

Control::~Control() {}

min_max_t Control::Calibrate() {
  int val = Read();
  if (val < _minMax.min) {
    _minMax.min = val;
  }

  if (val > _minMax.max) {
    _minMax.max = val;
  }

  return _minMax;
}

int Control::Read() {
  return analogRead(_pin);
}

int Control::ReadFilter(const int filterCount = 50) {
  unsigned int sum = 0;
  unsigned int i = 0;

  for (i = 0; i < filterCount; i++) {
    sum = sum + Read();
  }

  sum = sum / filterCount;
  return (sum);
}

void Control::SetMinMax(const min_max_t minMax) {
  _minMax.min = minMax.min;
  _minMax.max = minMax.max;
}

min_max_t Control::GetMinMax() {
  return _minMax;
}
