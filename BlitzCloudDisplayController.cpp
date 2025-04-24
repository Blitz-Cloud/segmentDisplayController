#include "BlitzCloudDisplayController.h"
#include <Arduino.h>
void DisplayLed::initializePins()
{
  if (hasShiftRegister)
  {
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
  }

  for (int i = 0; i < 8 && !hasShiftRegister; i++)
  {
    pinMode(devBoardGPIO[i], OUTPUT);
  }
}

void DisplayLed::updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

void DisplayLed::generateBinaryArray(int num, int binaryArray[8])
{
  for (int i = 7; i >= 0; --i)
  {
    int powerOfTwo = 1 << i;
    if (num >= powerOfTwo)
    {
      binaryArray[7 - i] = 1;
      num -= powerOfTwo;
    }
    else
    {
      binaryArray[7 - i] = 0;
    }
  }
}

DisplayLed::DisplayLed(int latch, int clock, int data, int hasRegister,
                       int *gpioPins)
    : latchPin(latch),
      clockPin(clock),
      dataPin(data),
      hasShiftRegister(hasRegister),
      devBoardGPIO(gpioPins)
{
  initializePins();
  leds = 0;
  if (hasRegister)
  {
    updateShiftRegister();
  }
}

void DisplayLed::resetDisplay()
{
  if (hasShiftRegister)
  {
    leds = 0;
    updateShiftRegister();
  }
  else
  {
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(devBoardGPIO[i], LOW);
    }
  }
}

void DisplayLed::showNumber(int num)
{
  resetDisplay();
  if (hasShiftRegister)
  {
    leds = numsInBinary[num];
    updateShiftRegister();
  }
  else
  {
    generateBinaryArray(numsInBinary[num], binaryBits);
    for (int i = 0; i < 8; i++)
    {
      if (binaryBits[i])
      {
        digitalWrite(devBoardGPIO[i], HIGH);
      }
    }
  }
}

void DisplayLed::showCharacter(char c)
{
  resetDisplay();
  if (hasShiftRegister)
  {
    leds = alphabetInBinary[int(toupper(c)) - 65];
    updateShiftRegister();
  }
  else
  {
    generateBinaryArray(int(toupper(c) - 65), binaryBits);
    for (int i = 0; i < 8; i++)
    {
      if (numsInBinary[i])
      {
        digitalWrite(devBoardGPIO[i], HIGH);
      }
    }
  }
}

DisplayLed::~DisplayLed() { delete devBoardGPIO; }

void DisplayLed2Digits::setState(int dis1, int dis2)
{
  if (dis1)
  {
    digitalWrite(gndDis1, LOW);
  }
  else
  {
    digitalWrite(gndDis1, HIGH);
  }
  if (dis2)
  {
    digitalWrite(gndDis2, LOW);
  }
  else
  {
    digitalWrite(gndDis2, HIGH);
  }
}

void DisplayLed2Digits::generateContentArray(unsigned int num, int *&length,
                                             int *&content)
{
  length = new int;
  *length = 0;
  int numCopy = num, k = 1;
  Serial.println("Lungime");
  Serial.println(numCopy);
  while (num > 0)
  {
    num /= 10;
    *length = *length + 1;
  }
  Serial.println(numCopy);
  num = numCopy;
  content = new int[*length];
  while (num)
  {
    content[*length - k] = num % 10;
    k++;
    num /= 10;
  }
}

DisplayLed2Digits::DisplayLed2Digits(int latch, int clock, int data, int hasRegister,
                                     int *gpio, int gndD1, int gndD2)
    : display(latch, clock, data, hasRegister, gpio),
      gndDis1(gndD1),
      gndDis2(gndD2)
{
  pinMode(gndD1, OUTPUT);
  pinMode(gndD2, OUTPUT);
}

void DisplayLed2Digits::showNumberOld(int num)
{
  contentLength = 0;
  if (num < 10)
  {
  }
  else
  {
    int numCopy = num;
    while (num)
    {
      num /= 10;
      contentLength++;
    }
    int *content = new int[contentLength];
    num = numCopy;
    int k = 0;
    while (num)
    {
      content[contentLength - k - 1] = num % 10;
      k++;
      num /= 10;
    }
    static unsigned long time = millis();
    for (pozArray = 0; pozArray < contentLength; pozArray = pozArray + 2)
    {
      Serial.println(content[pozArray]);
      while (millis() - time <= 500)
      {
        display.resetDisplay();
        setState(1, 0);
        display.showNumber(content[pozArray]);
        delay(5);
        display.resetDisplay();
        setState(0, 1);

        display.showNumber(content[pozArray + 1]);
        delay(5);
      }
    }
    delete[] content;
  }
}

void DisplayLed2Digits::showNumber(unsigned int num, int duration)
{
  int i, *length = nullptr, *content = nullptr;
  generateContentArray(num, length, content);
  Serial.println(*length);
  Serial.println();

  for (i = 0; i < *length - *length % 2; i = i + 2)
  {
    unsigned long time = millis();
    while (millis() - time < 1000)
    {
      display.resetDisplay();
      setState(1, 0);
      display.showNumber(content[i]);
      delay(5);
      display.resetDisplay();
      setState(0, 1);
      display.showNumber(content[i + 1]);
      delay(5);
    }
  }
  if (i == *length - 1)
  {
    unsigned long time = millis();
    while (millis() - time < 1000)
    {
      display.resetDisplay();
      setState(0, 1);
      display.showNumber(content[i]);
    }
  }
  setState(0, 0);

  delete length;
  delete[] content;
  delay(2500);
}