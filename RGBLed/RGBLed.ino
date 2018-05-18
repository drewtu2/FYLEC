// Struct for RGB colors
struct RGB {
  byte r;
  byte g;
  byte b;
};

//LEDs are connected to these pins
int RedPin = 6;
int GreenPin = 5;
int BluePin = 3;

// Other Variables
int NUM_STEPS = 100; // how smooth do we want the transition between colors. 
int SLEEP_TIME = 10; // time in seconds
int SECOND = 1000;
int LED_MAX = 255;
int LED_MIN = 0;

// RGB Colors
RGB red = { LED_MAX , LED_MIN , LED_MIN };
RGB white = { LED_MAX , LED_MAX , LED_MAX};
RGB green = { LED_MIN , LED_MAX , LED_MIN };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);           //start a serial connection with the computer

  //set the LED pins to output
  pinMode(RedPin,OUTPUT);
  pinMode(GreenPin,OUTPUT);
  pinMode(BluePin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  changeColor(red, white, 5);
  delay(1 * SECOND);
  changeColor(white, green, 5);
  delay(5 * SECOND);
  changeColor(green, white, 5);
  delay(1 * SECOND);
  changeColor(white, red, 5);
  delay(5 * SECOND);

}

void changeColor(RGB c1, RGB c2, int duration) {
  // Takes two colors and transitions from the first color to the second color
  // arg duration: the number of seconds to transition
  int i = 0; 
  int steps = duration * SECOND;

  while(i < steps) {
    analogWrite(RedPin, interpolate(c1.r, c2.r, 0, steps, i));
    analogWrite(GreenPin, interpolate(c1.g, c2.g, 0, steps, i));
    analogWrite(BluePin, interpolate(c1.b, c1.b, 0, steps, i));
    i++;
    delay(1);
  }
}

float interpolate(
    float startValue,
    float endValue,
    int startTime,
    int endTime,
    int timeNow) {
  // Transition between two values. 
  
  float componentA = startValue * (endTime - timeNow) / (endTime - startTime);
  float componentB = endValue * (timeNow - startTime) / (endTime - startTime);

  return componentA + componentB;
}


