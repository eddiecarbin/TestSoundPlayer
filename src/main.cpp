#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "PotMonitor.h"
#include <JC_Button.h>

#define BUTTON_PIN 3
#define POT_PIN A0

PotMonitor volumePot(POT_PIN, 12);

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

Button changeButton(BUTTON_PIN, 25, false);

int fileCount = 0;
int soundIdx = 1;

long map2(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

void setup()
{
  // put your setup code here, to run once:

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  delay(3000);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  changeButton.begin();
  // pinMode(BUTTON_PIN, INPUT);

  if (!myDFPlayer.begin(mySoftwareSerial))
  { //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(15); //Set volume value. From 0 to 30

  fileCount = myDFPlayer.readFileCounts();
  Serial.println(fileCount);
  Serial.print("file count = ");
}

void StartSoundPlayer()
{
  myDFPlayer.stop();

  delay(10);
  myDFPlayer.play(soundIdx); //Play the first mp3
  soundIdx++;
  if (soundIdx > fileCount)
  {
    soundIdx = 1;
  }
}

void StopSoundPlayer()
{
  myDFPlayer.stop();
}

void loop()
{
  // put your main code here, to run repeatedly:

  // volumePot.hasUpdated();
  if (volumePot.hasUpdated())
  {
    //Serial.println(speedPotentiometer.getValue());
    long volume = map2(volumePot.getValue(), 0, 1024, 0, 30);
    Serial.println(volume);
    myDFPlayer.volume(volume);
  }

  changeButton.read();
  if (changeButton.wasReleased())
  {
    Serial.println("was pressed");
    StartSoundPlayer();
  }
  // Serial.println(digitalRead(BUTTON_PIN));
  // if (debouncer.read() == LOW && soundPlaying == false)
  // {
  //
  // }

  // if (debouncer.read() == HIGH && soundPlaying == true)
  // {
  //   StopSoundPlayer();
  // }
}