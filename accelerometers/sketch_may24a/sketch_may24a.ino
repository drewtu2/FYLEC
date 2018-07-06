
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

// For inverted colors (Using N Channel Mosfets)
// int LED_MAX = 0;
// int LED_MIN = 255;

// For Normal LED or P Channel Mosfet
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
Accelerometer scaled;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200); 
  
  scale = 10;
  velX = 0; // Initial velocity is zero
  
  //set the LED pins to output
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  raw = {analogRead(A0), analogRead(A1), analogRead(A2)};

  // Get the instantaneous velocity value
  // (Using an Accelerometer value to represent the velocity because it has
  // X, Y, Z components... should change the name of the struct...)
  raw = getVelocity(10, 5);
  
  // Increment the running tally of the velocity.
  velX += raw.x;
 
  // Print some values so we have read out
  Serial.print("Accel: "); Serial.print(raw.x); Serial.println(" m/s^2");
  Serial.print("Accum Vel: "); Serial.println(velX); Serial.println(" m/s");;
  //printAccel("Scaled Values", scaled);

  // Set the color of the LED using the current acceleration
  accel2Color(velX);
}

void printAccel(String title, Accelerometer value) {
  // Simple helper function for printing a labeled accelerometer value
  // @param title: Title of the value to be printed
  // @param value: an accelerometer struct with the X, Y, and Z values to be printed. 

  Serial.println("********************");
  Serial.println(title);
  Serial.println("********************");
  Serial.print("X: "); Serial.println(value.x);
  Serial.print("Y: "); Serial.println(value.y);
  Serial.print("Z: "); Serial.println(value.z);
}

Accelerometer scaleAccel(Accelerometer value, float inMin, float inMax, float scale) {
  // Returns a linear scaling of the given accelerometer values based on a set of input. 
  // @param value: the accelerometer valeus to scale
  // @param inMin: a float representing the lowest possible input value
  // @param inMax: a flaot representing the max possible input value
  // @param scale: the scale to put the output on (-scale, +scale)

  Accelerometer scaled;
  scaled.x = mapf(value.x, inMin, inMax, -scale, scale); // 3.3/5 * 1023 =~ 675
  scaled.y = mapf(value.y, inMin, inMax, -scale, scale); // 3.3/5 * 1023 =~ 675
  scaled.z = mapf(value.z, inMin, inMax, -scale, scale); // 3.3/5 * 1023 =~ 675
  
  return scaled;
}

Accelerometer getVelocity(int numReadings, int deltaT) {
  // Returns an accelerometer value holding the given velocity calcualted from
  // taking a given number of readings with deltaT time between each reading. 
  // Total duraiton of the sample time is numReadings * deltaT
  // @param numReadings: the number of readings to take 
  // @param deltaT: the amount of time between each reading (in ms)

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
  // The map function for floats

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
  // Takes a float value and changes the color of the LEDs using those values. 
  // TODO: Should update to take rangeMin/rangeMax and colors as arguments...
  
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
