#include <wiringPi.h>
#include "../include/aquamatic.h"

#define RELAY_PIN 0

inline void heater_on(void)
{
        digitalWrite(RELAY_PIN, HIGH);
}

inline void heater_off(void)
{
        digitalWrite(RELAY_PIN, LOW);
}

void control_heater(float temperature)
{
        
};
