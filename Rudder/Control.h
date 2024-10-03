//####################################################
//      Includes
//####################################################

//####################################################
//      Constants
//####################################################

//####################################################
//      Types
//####################################################
typedef struct
{
  int min;
  int max;
} min_max_t;

class Control {
public:
  Control(int pin);
  ~Control();

  min_max_t Calibrate();
  int Read();
  int ReadFilter(const int filterCount = 50);
  void SetMinMax(const min_max_t minMax);
  min_max_t GetMinMax();

  static const int DEFAULT_MIN_MAX = 512;

private:
  int _pin;
  min_max_t _minMax;
};
