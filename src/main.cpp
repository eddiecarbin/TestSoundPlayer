#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "PotMonitor.h"
#include <JC_Button.h>

#define BUTTON_PIN 3
#define POT_PIN 18

PotMonitor volumePot(POT_PIN, 12);

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

Button changeButton(BUTTON_PIN, 25, false);

int fileCount = 0;
int playCount = 0;
bool soundPlaying = false;
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

  Serial.print("file count = ");
  Serial.println(fileCount);

  delay(500);
}

void StartSoundPlayer()
{
  soundPlaying = true;
  playCount = 1;
  delay(500);
  myDFPlayer.play(soundIdx); //Play the first mp3
  soundIdx++;
  if (soundIdx > fileCount)
  {
    soundIdx = 1;
  }
}

void StopSoundPlayer()
{
  soundPlaying = false;
  myDFPlayer.stop();
}

void loop()
{
  // put your main code here, to run repeatedly:

  // volumePot.hasUpdated();
  if (volumePot.hasUpdated())
  {
    //Serial.println(speedPotentiometer.getValue());
    long volume = map2(volumePot.getValue(), 0, 880, 0, 30);
    Serial.println(volume);
    myDFPlayer.volume(volume);
  }

  if (myDFPlayer.available() && soundPlaying == true)
  {

    if (myDFPlayer.readType() == DFPlayerPlayFinished && playCount < fileCount)
    {

      delay(500);
      myDFPlayer.next();
      soundIdx++;

      if (soundIdx > fileCount)
      {
        soundIdx = 1;
      }
      Serial.print("count = ");
      Serial.println(playCount);

      playCount += 1;
    }
    else
    {
    }
  }

  changeButton.read();
  // Serial.println(digitalRead(BUTTON_PIN));
  // if (debouncer.read() == LOW && soundPlaying == false)
  // {
  //   StartSoundPlayer();
  // }

  // if (debouncer.read() == HIGH && soundPlaying == true)
  // {
  //   StopSoundPlayer();
  // }
}