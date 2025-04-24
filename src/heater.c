#include <wiringPi.h>
#include "../include/aquamatic.h"


inline void heater_on(void)
{
        digitalWrite(HEATER_PIN, LOW);
}

inline void heater_off(void)
{
        digitalWrite(HEATER_PIN, HIGH);
}

static inline void __control_heater(float temperature, setpoint_t sp)
{
        if (temperature < sp.temp - 0.5) {
                heater_on();
        } else if (temperature > sp.temp + 0.5) {
                heater_off();
        }
        return;
}

static inline void __control_heater_log(float temperature, setpoint_t sp)
{
        FILE *log_file = fopen(STATE_FILE, "a");
        if (!log_file) {
                perror("Error opening log file");
                return;
        }

        if (temperature < sp.temp - 0.5) {
                heater_on();
                fprintf(log_file, "heater on\n");
        } else if (temperature > sp.temp + 0.5) {
                heater_off();
                fprintf(log_file, "heater_off\n");
        }
        fflush(log_file);
        return;
}

void control_heater(float temperature)
{
        __control_heater(temperature, sp);
}
