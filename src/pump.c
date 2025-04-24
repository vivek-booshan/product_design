#include <wiringPi.h>
#include "../include/aquamatic.h"

static inline void ph_up_on(void)
{
        digitalWrite(PHUP_PIN, HIGH);
}

static inline void ph_up_off(void)
{
        digitalWrite(PHUP_PIN, LOW);
}

static inline void ph_down_on(void)
{
        digitalWrite(PHDOWN_PIN, HIGH);
}

static inline void ph_down_off(void)
{
        digitalWrite(PHDOWN_PIN, LOW);
}

// if outside ph range, turn on for n seconds
inline void control_ph(float ph)
{
        int n = 1;
        if (ph < sp.ph - 0.1) {
                ph_up_on();
                sleep(n);
        } else if (ph > sp.ph + 0.1) {
                ph_down_on();
                sleep(n);
        }
        ph_up_off();
        ph_down_off();
}
