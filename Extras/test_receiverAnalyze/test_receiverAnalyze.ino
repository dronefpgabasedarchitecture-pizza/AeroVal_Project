#include "Arduino.h"
#include <globalVariables.h>
#include <globalVariables.h>
#include <receiver.h>

void receiver_analyse(void);
void setup()
{
 Serial.begin(115200);
 pinMode(RED_LED, OUTPUT);
 pinMode(STATE_LED, OUTPUT);
 pinMode(MODE_LED, OUTPUT);
 receiver_init();
}

void loop()
{
  receiver_analyse();
  digitalWrite(13, armed);
  if (!armed)
  {
         digitalWrite(RED_LED, LOW);
  }
  else
  {
         digitalWrite(RED_LED, HIGH);
  }
}



