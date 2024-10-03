//####################################################
//      Includes
//####################################################
#include "Control.h"
#include <Joystick.h>

//####################################################
//      Constants
//####################################################

//####################################################
//      Types
//####################################################
typedef struct
{
  min_max_t rudder;
  min_max_t leftBrake;
  min_max_t rightBrake;
} cal_values_t;

class RudderPedal {
public:
  RudderPedal(int rudderPin, int leftBrakePin, int rightBrakePin);
  ~RudderPedal();

  void Init();
  void UpdateControls();
  void StartCal();
  void Calibrate();
  void WriteCalValues();

  void PrintMinMax();

private:
  void UpdateJoystickRange(const cal_values_t values);

  Control _rudder;
  Control _leftBrake;
  Control _rightBrake;

  Joystick_ _joystick;

  const int FILTER_COUNT = 50;
};
