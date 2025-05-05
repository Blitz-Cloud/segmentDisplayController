# Librarie pentru a usura utilizarea unui ecran cu 8 segmente

## Instalarea librarie:

- Folosind Arduino IDE:

1. Se descarca codul sursa sub forma unei arhive .zip. Se apasa butonul **CODE** > **DOWNLOAD ZIP**.
2. Apoi in Arduino IDE se selecteaza **SKETCH** > **INCLUDE LIBRARY** > **MANAGE LIBRARIES** si se selecteaza fisierul zip ce tocmai a fost descarcat.

- Pentru VSCode utilizand PlatformIO:
  > Disclaimer: este ceva mai complicat insa trebuie folosit CLI ul celor de la PlatformIO

1. In meniul extesiei **PlatformIO**, in dropdown ul de **Quick Access** se selecteaza **PlatformIO Core CLI**.
2. Se ruleaza commanda de mai jos unde PATH este **locatia** fisierului **.zip**.

```bash
$ pio pkg install -l PATH
```

## Utilizare:

1. Fara registru de shiftare:

```cpp
#include <BlitzCloudDisplayController.h>

// in vectorul gpioPins sunt sunt puse in ordine firele pentru display-ul cu 8 segmente
// pentru a avea o functionare corecta trebuie respectata ordinea urmatoare:
// Punctul, Jos Dreapta,Jos Mijloc,Jos Stanga,Sus stanga, Sus mijloc,Sus dreapta,Mijloc
// unde fiecare dintre acestea reprezinta segmentul din ecran care este
// controlat de pinul respectiv
int gpioPins[8] = {11, 10, 9, 8, 7, 6, 5, 4};
// catodu pentru prima cifra a ecranului cu 8 segmente
int dis1 = 2;
// catodu pentru prima a doua cifra a ecranului cu 8 segmente
int dis2 = 3;
DisplayLed2Digits display{0, 0, 0, 0, gpioPins, dis1, dis2};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  display.showNumber(213);
}

```

2. Cu registru de [shiftare 595](https://www.ti.com/lit/ds/symlink/sn74hc595.pdf):

```cpp
#include <BlitzCloudDisplayController.h>

int dis1 = 2;
int dis2 = 3;
int *gpio=nullptr;

Display2Digits display{5, 6, 4, 1, gpio,dis2, dis1};

// pinul 5 este latch Pinul
// pinul 6 este clock Pinul
// pinul 4 este data Pinul

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  disp.showNumber(190);
  delay(2000);
}
```

3. De asemenea in cazul in care se foloseste un display cu 8 segmente doar cu un singur digit, atunci se poate folosi clasa **DisplayLed** ca in exemplul urmator, acest exemplu folosind registrul de shiftare 595 :

```cpp

#include <BlitzCloudDisplayController.h>

int dis1 = 2;
int *gpio=nullptr;

DisplayLed display{5, 6, 4, 1};

// pinul 5 este latch Pinul
// pinul 6 este clock Pinul
// pinul 4 este data Pinul

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // este posibila doar afisarea numerelor de la 0 la 9 folosind aceasta varianta
  display.showNumber(1);
  delay(2000);
}
```

4. Alte utilizari:

```cpp

Display2Digits display(.......)
// daca se doreste afisarea pe ecran a sirului de caractere abc
display.showCharacter('abc');
// de asemenea este posibila si afisarea la ab12
display.showCharacter('ab12');

```
