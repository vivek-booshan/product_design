#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <time.h>
#include <pthread.h>

#define FILE_PATH "./temp.txt"
#define cast(Type, Value) (Type)(Value)
typedef unsigned int uint;

typedef enum {
    false = 0,
    true
} bool;

#define TEMP_LOG "/tmp/aquamatic/temperature.txt"
#define HEATER_STATE "/tmp/aquamatic/heater_state.txt"

// Define control setpoints
#define TEMP_SETPOINT_LOW 20.0  
#define TEMP_SETPOINT_HIGH 22.0
#define RANDOM_DRIFT 0.02
#define HEAT_RATE_MIN 0.12
#define HEAT_RATE_MAX 0.18
#define COOL_RATE_MIN 0.05
#define COOL_RATE_MAX 0.09

int heater_state = 0;

void simulate_heater_temperature(const char *filename);
void control_heater(void);
float get_latest_temperature(void);
int main(void)
{
    /* FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char line[100];
    double T_median;

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char *date = strtok(line, " ");
        char *time = strtok(NULL, ",");
        char *temp = strtok(NULL, "\n");

        if (temp)  T_median = atof(temp);
        
    }

    fclose(file); */

    srand(cast(uint, time(NULL)));
    pthread_t temp_thread, heater_thread;
    pthread_create(&temp_thread, NULL, cast(void *, simulate_heater_temperature), cast(void *, "/tmp/aquamatic/temperature.txt"));
    pthread_create(&heater_thread, NULL, cast(void *, control_heater), NULL);

    pthread_join(temp_thread, NULL);
    pthread_join(heater_thread, NULL);
    return 0;
}

// Retrieve a timestamp of the current local time as YY-MM-DD HH:MM:SS
static inline void get_timestamp(char *buffer, size_t buffer_size)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime); 

    strftime(buffer, buffer_size, "%y-%m-%d %H:%M:%S", timeinfo);
}

// Simulate random drift using min + (rand / RAND_MAX) * (max - min)
static inline float random_drift(const float min, const float max)
{
    return min + (cast(float, rand()) / cast(float, RAND_MAX)) * (max - min);
}

/*
Simulates a heater generating temperatures between 18 and 25 Celsius. A
temperature is generated based on random_drift + a heating rate based on if the
heater state (global variable) is on or off. The temperature is written to file
and logged.
*/
void simulate_heater_temperature(const char *filename)
{
    // float min_temp = 18.0f;
    // float max_temp = 25.0f;

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    if (flock(fileno(file), LOCK_EX) != 0) {
        perror("Error locking file");
        fclose(file);
        return;
    }
    float temperature = get_latest_temperature();
    while (1) {

        if (heater_state) 
            temperature += random_drift(cast(float, HEAT_RATE_MIN), cast(float, HEAT_RATE_MAX));
        else 
            temperature -= random_drift(cast(float, COOL_RATE_MIN), cast(float, COOL_RATE_MAX));
        
        temperature += random_drift(-cast(float, RANDOM_DRIFT), cast(float, RANDOM_DRIFT));

        char timestamp[20];
        get_timestamp(timestamp, sizeof(timestamp));

        fprintf(file, "%s, %.2f\n", timestamp, temperature);
        fflush(file);

        printf("Logged temperature: %.2f at %s\n", temperature, timestamp);

        sleep(1);
    }

    flock(fileno(file), LOCK_UN);
    fclose(file);
}

// Function to read the most recent temperature from the log file
float get_latest_temperature(void)
{
    FILE *file = fopen(TEMP_LOG, "r");
    if (!file) {
        perror("Error opening temperature log");
        return -1;
    }

    char line[100];
    char last_line[100];

    // Read the last line of the file
    short i = 0;
    while (fgets(line, sizeof(line), file)) {
        strcpy(last_line, line);
        i++;
    }

    if (i >= 1000) {
        truncate(TEMP_LOG, 0);
        freopen(TEMP_LOG, "w", file);
        fprintf(file, "%s", last_line);
        fflush(file);
    }

    fclose(file);

    // Extract the temperature value from the last line
    char *token = strrchr(last_line, ','); // Find last comma
    if (!token) {
        fprintf(stderr, "Error parsing temperature data\n");
        return -1;
    }
    return cast(float, atof(token + 1)); // Convert temperature string to float
}

// Function to control the heater based on temperature
void control_heater(void)
{

    while (1) {
        float current_temp = get_latest_temperature();
        if (current_temp < 0) {
            fprintf(stderr, "Failed to read temperature, retrying...\n");
            sleep(5);
            continue;
        }

        // Determine heater state
        if (current_temp < TEMP_SETPOINT_LOW) {
            heater_state = 1; // Turn ON
        } else if (current_temp > TEMP_SETPOINT_HIGH) {
            heater_state = 0; // Turn OFF
        }

        // Write heater state to file
        /*
        FILE *file = fopen(HEATER_STATE, "w");
        if (!file) {
            perror("Error opening heater state file");
        } else {
            // Lock file to ensure safe writing
            if (flock(fileno(file), LOCK_EX) == 0) {
                fprintf(file, "%d\n", heater_state);
                fflush(file);
                flock(fileno(file), LOCK_UN);
            } else {
                perror("Error locking heater state file");
            }
            fclose(file);
        }
        */

        // Print for debugging
        printf("Temperature: %.2f°C -> Heater %s\n", current_temp, heater_state ? "ON" : "OFF");

        // Check temperature every 5 seconds
        sleep(1);
    }
}

