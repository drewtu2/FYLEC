
// Struct for Accelerometer Data
struct Accelerometer {
  float x;
  float y;
  float z; 
};

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
// For inverted colors (Using P Channel Mosfets)
// int LED_MAX = 0;
// int LED_MIN = 255;

// For Normal LED
int LED_MAX = 255;
int LED_MIN = 0;

// RGB Colors
RGB red = { LED_MAX , LED_MIN , LED_MIN };
RGB white = { LED_MAX , LED_MAX , LED_MAX};
RGB green = { LED_MIN , LED_MAX , LED_MIN };

bool micro_is_5V = false;
float scale;
float velX;

Accelerometer raw;
Accelerometer prev;
Accelerometer scaled;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  scale = 10;
  velX = 0;

  prev = {0, 0, 0};
  
  //set the LED pins to output
  pinMode(RedPin,OUTPUT);
  pinMode(GreenPin,OUTPUT);
  pinMode(BluePin,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  raw = {analogRead(A0), analogRead(A1), analogRead(A2)};

  raw = getVelocity(10, 5);
  velX += raw.x;
  
  Serial.print("Accel: "); Serial.print(raw.x); Serial.println(" m/s^2");
  Serial.print("Accum Vel: "); Serial.println(velX); Serial.println(" m/s");;
  //printAccel("Scaled Values", scaled);
  accel2Color(velX);
}

void printAccel(String title, Accelerometer value) {
  Serial.println("********************");
  Serial.println(title);
  Serial.println("********************");
  Serial.print("X: "); Serial.println(value.x);
  Serial.print("Y: "); Serial.println(value.y);
  Serial.print("Z: "); Serial.println(value.z);
}

Accelerometer scaleAccel(Accelerometer value, float inMin, float inMax, float scale) {
  Accelerometer scaled;
  scaled.x = mapf(value.x, inMin, inMax, -scale, scale); // 3.3/5 * 1023 =~ 675
  scaled.y = mapf(value.y, inMin, inMax, -scale, scale); // 3.3/5 * 1023 =~ 675
  scaled.z = mapf(value.z, inMin, inMax, -scale, scale); // 3.3/5 * 1023 =~ 675
  return scaled;
}

Accelerometer getVelocity(int numReadings, int deltaT) {
  Accelerometer accum = {0, 0, 0};
  Accelerometer raw;
  for(int readingNum = 0; readingNum < numReadings; readingNum++) {  
    raw = {analogRead(A0), analogRead(A1), analogRead(A2) };
    raw = scaleAccel(raw, 350, 650, scale); 
    accum.x += raw.x;
    accum.y += raw.y;
    accum.z += raw.z;
    delay(deltaT);
  }

  float dt = (float)deltaT / SECOND;

  return {accum.x * dt, accum.y * dt, accum.z * dt};
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{

  // Handle out of bounds cases
  if(x < in_min) {
    return out_min;
  } else if (x > in_max) {
    return out_max;
  }

  // Handle normal boundary case
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void accel2Color(float value) {
  float rangeMin = 1;
  float rangeMax = 2;
  float offset = 0;
  
  RGB c1 = white;
  RGB c2 = green;

  if (value < 0) {
    c1 = red;
    c2 = white;
    rangeMin = 0;
    rangeMax = 1;
  }
  
  analogWrite(RedPin, mapf(value, rangeMin + offset, rangeMax + offset, c1.r, c2.r));
  analogWrite(GreenPin, mapf(value, rangeMin + offset, rangeMax + offset, c1.g, c2.g));
  analogWrite(BluePin, mapf(value, rangeMin + offset, rangeMax + offset, c1.b, c2.b));
}
