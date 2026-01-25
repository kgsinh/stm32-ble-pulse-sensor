#ifndef PULSE_H
#define PULSE_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

// Pulse detection constants
#define PULSE_THRESHOLD_RATIO 0.5f
#define PULSE_HYSTERESIS 50
#define BEAT_HISTORY_SIZE 8
#define SIGNAL_HISTORY_SIZE 20
#define MIN_BEAT_INTERVAL_MS 333     // Max 180 BPM
#define MAX_BEAT_INTERVAL_MS 1500    // Min 40 BPM
#define RESET_INTERVAL_MS 2000       // Signal reset timeout
#define MIN_SIGNAL_AMPLITUDE 20      // Minimum signal strength

// Function prototypes
void pulse_sensor_init(void);
void pulse_sensor_process(uint16_t adc_value);
bool pulse_sensor_beat_detected(void);
uint16_t pulse_sensor_get_bpm(void);
bool pulse_sensor_bpm_valid(void);
uint8_t get_signal_quality(void);

// Additional diagnostic functions
uint16_t get_signal_amplitude(void);
uint16_t get_signal_min(void);
uint16_t get_signal_max(void);

#endif // PULSE_H

