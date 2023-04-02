#include "pitches.h"
#include "odeToJoy.h"


float scale = 0;

void setup() {



}

void loop() {
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(int); thisNote++) {
    scale = analogRead(A0) / 512.0;
    int noteDuration = 1000 / noteDurations[thisNote];

    tone(8, (int)(melody[thisNote]*scale), noteDuration);

    delay(noteDuration * 1.30);

  }
}
