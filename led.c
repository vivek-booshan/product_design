#include <stdio.h>
#include <wiringPi.h> 

#define LedPin 0

int main() {
   if (wiringPiSetup() == -1) {
      printf("setup wiringPi failed !");
      return 1;
   }
   pinMode(LedPin, OUTPUT);
   while(1) {
      digitalWrite(LedPin, LOW);
      printf("...LED on\n");
      delay(500);
      digitalWrite(LedPin, HIGH);
      printf("Led off...\n");
      delay(500);
   }
   return 0;
}
