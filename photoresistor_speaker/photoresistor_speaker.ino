/*
SparkFun Inventor’s Kit
Circuit 1C-Photoresistor

Use a photoresistor to monitor how bright a room is, and turn an LED on when it gets dark.

This sketch was written by SparkFun Electronics, with lots of help from the Arduino community.
This code is completely free for any use.

View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40
Download drawings and code at: https://github.com/sparkfun/SIK-Guide-Code
*/

int photoresistor = 0;              // this variable will hold a value based on the position of the knob
int photo_max = 1024;               // photo_max
int photo_min = 500;
int photo_range = photo_max-photo_min; // the actual reachable range of the photo sensor in this environment
int threshold = 750;                // if the photoresistor reading is below this value the the light will turn on
int speakerPin = 13;                // the pin that buzzer is connected to

void setup()
{
  Serial.begin(9600);               //start a serial connection with the computer
  pinMode(speakerPin, OUTPUT);    //set the output pin for the speaker
}

void loop()
{
  //read the position of the knob
  photoresistor = analogRead(A0);   //set photoresistor to a number between 0 and 1023 based on how far the knob is turned
  //Serial.println(photoresistor);    //print the value of photoresistor in the serial monitor on the computer

  play(photoresistor, 1);

  delay(100);                       //short delay to make the printout easier to read
}

void play( int photoresistor, int beats)
{
  int numNotes = 14;  // number of notes in our note and frequency array (there are 15 values, but arrays start at 0)
  
  //Note: these notes are C major (there are no sharps or flats)

  //this array is used to look up the notes
  char notes[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B', ' '};
  //this array matches frequencies with each letter (e.g. the 4th note is 'f', the 4th frequency is 175)
  int frequencies[] = {131, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 0};
  
  int currentFrequency = 0;    //the frequency that we find when we look up a frequency in the arrays
  int beatLength = 150;   //the length of one beat (changing this will speed up or slow down the tempo of the song)

  //look up the frequency that corresponds to the note
  /*for (int i = 0; i < numNotes; i++)  // check each value in notes from 0 to 14
  {
    if (notes[i] == note)             // does the letter passed to the play function match the letter in the array?
    {
      currentFrequency = frequencies[i];   // Yes! Set the current frequency to match that note
    }
  }*/
  int frequency_index = ((float) numNotes / photo_range) * max(0, (photoresistor - photo_min));
  currentFrequency = frequencies[frequency_index];

  Serial.println("**********************");
  Serial.print("Photovalue: ");
  Serial.println(photoresistor);
  Serial.print("Frequency: ");
  Serial.println(currentFrequency);
  Serial.print("Frequency Index: ");
  Serial.println(frequency_index);
  Serial.println("**********************\n");
  

  //play the frequency that matched our letter for the number of beats passed to the play function
  tone(speakerPin, currentFrequency, beats * beatLength);   
  delay(beats* beatLength);   //wait for the length of the tone so that it has time to play
  delay(50);                  //a little delay between the notes makes the song sound more natural

}

/* CHART OF FREQUENCIES FOR NOTES IN C MAJOR
Note      Frequency (Hz)
c        131
d        147
e        165
f        175
g        196
a        220
b        247
C        262
D        294
E        330
F        349
G        392
A        440
B        494
*/

