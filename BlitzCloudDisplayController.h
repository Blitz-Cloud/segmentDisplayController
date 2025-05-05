#ifndef BlitzCloudDisplayLedController_h
#define BlitzCloudDisplayLedController_h
#include <Arduino.h>

class DisplayLed
{
private:
  byte leds;
  int latchPin, clockPin, dataPin;
  int *devBoardGPIO;
  int binaryBits[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int numsInBinary[10] = {126, 72, 61, 109, 67, 103, 119, 76, 127, 79};
  int alphabetInBinary[27] = {
      95, 115, 54, 121, 55, 23, 119, 83, 18, 120, 0, 50, 0,
      0, 126, 31, 79, 17, 103, 51, 122, 0, 0, 91, 75,
      61};                  // sunt reprezentate literele alfabetului de la a la z,(
                            // exceptii fiind k,m,n,v si w) ca suma de puteri ale lui 2 dupa
                            // cum sunt asociate segmentele ecranului cu led uri ca puteri ale
                            // lui 2
  int hasShiftRegister = 0; // libraria a fost constuita cu 74HC595 ca
                            // registru de shiftare

  void initializePins();
  void updateShiftRegister();
  void generateBinaryArray(int num, int binaryArray[8]);

public:
  DisplayLed(int latch = 0, int clock = 0, int data = 0, int hasRegister = 0,
             int *gpioPins = nullptr);
  void resetDisplay();
  void showNumber(int num);
  void showCharacter(char c);
  ~DisplayLed();
};

class DisplayLed2Digits
{
private:
  DisplayLed display;
  int gndDis1, gndDis2;
  int contentLength;
  int pozArray;

  void setState(int dis1, int dis2);
  void generateContentArray(unsigned int num, int *&contentLength,
                            int *&content);

public:
  DisplayLed2Digits(int latch, int clock, int data, int hasRegister, int *gpio,
                    int gndD1, int gndD2);
  void showNumber(unsigned int num, int duration = 1000);
  void showCharacter(char c[], int duration = 1000);
};
#endif
