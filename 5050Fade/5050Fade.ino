int brightness = 0;

int gBright = 255;
int rBright = 255;
int bBright = 255;

int fadeSpeed = 10;
int RED_LED = 6;
int GREEN_LED = 5;
int BLUE_LED = 3;

void setup() {
  // put your setup code here, to run once:
   pinMode(GREEN_LED, OUTPUT);
   pinMode(RED_LED, OUTPUT);
   pinMode(BLUE_LED, OUTPUT);
   Serial.begin(9600);
   Serial.print("Hello World");
}

void loop() {
  // put your main code here, to run repeatedly:

  TurnOn();
  delay(5000);
  TurnOff();
}

void TurnOn() { 
   for (int i = 0; i < 255; i++) {
       analogWrite(RED_LED, rBright);
       rBright -=1;
       delay(fadeSpeed);
   }
  printValues();
   for (int i = 0; i < 255; i++) {
       analogWrite(BLUE_LED, bBright);
       bBright -= 1;
       delay(fadeSpeed);
   } 
  printValues();
   for (int i = 0; i < 255; i++) {
       analogWrite(GREEN_LED, gBright);
       gBright -=1;
       delay(fadeSpeed);
   } 
  // Everything should be at 0 (on)
  brightness = 0;
  printValues();
}

void TurnOff() {
   for (int i = 0; i < 255; i++) {
       analogWrite(GREEN_LED, brightness);
       analogWrite(RED_LED, brightness);
       analogWrite(BLUE_LED, brightness);
 
       brightness += 1;
       delay(fadeSpeed);

   }
  // Brightness is at 255 (off)
  // Reset values of colors to be 255 (off)
  rBright = 255;
  gBright = 255;
  bBright = 255;
  printValues();
}

void printValues() {
  Serial.print("R: ");
  Serial.println(rBright);
  Serial.print("G: ");
  Serial.println(gBright);
  Serial.print("B: ");
  Serial.println(bBright);
  Serial.print("Brightness: ");
  Serial.println(brightness);
}

