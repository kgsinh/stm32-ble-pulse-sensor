#include "pulse_sensor.h"
#include <stdio.h>

// Simple constants
#define BEAT_THRESHOLD 2048        // Fixed threshold (middle of 12-bit range)
#define HYSTERESIS 100             // Prevent noise triggering
#define MIN_BEAT_INTERVAL_MS 333   // Max 180 BPM
#define MAX_BEAT_INTERVAL_MS 1500  // Min 40 BPM
#define HISTORY_SIZE 4             // Small history for faster response

// State variables
static uint32_t last_beat_time = 0;
static uint32_t beat_intervals[HISTORY_SIZE];
static uint8_t interval_count = 0;
static uint16_t current_bpm = 0;
static bool above_threshold = false;
static bool new_beat = false;

// Private functions declarations
static void calculate_bpm(void);


void pulse_sensor_init(void) {
    last_beat_time = 0;
    interval_count = 0;
    current_bpm = 0;
    above_threshold = false;
    new_beat = false;
}

void pulse_sensor_process(uint16_t adc_value) {
    uint32_t now = HAL_GetTick();

    // Simple rising edge detection
    if (!above_threshold && adc_value > (BEAT_THRESHOLD + HYSTERESIS)) {
        above_threshold = true;

        // Record beat timing
        if (last_beat_time > 0) {
            uint32_t interval = now - last_beat_time;

            // Validate interval is reasonable
            if (interval >= MIN_BEAT_INTERVAL_MS && interval <= MAX_BEAT_INTERVAL_MS) {
                beat_intervals[interval_count % HISTORY_SIZE] = interval;
                interval_count++;

                // Calculate BPM when we have enough data
                if (interval_count >= 2) {
                    calculate_bpm();
                    new_beat = true;
                    printf("Beat! Interval: %lu ms, BPM: %d\n\r", interval, current_bpm);
                }
            }
        }
        last_beat_time = now;
    }

    // Falling edge detection
    if (above_threshold && adc_value < (BEAT_THRESHOLD - HYSTERESIS)) {
        above_threshold = false;
    }
}

static void calculate_bpm(void) {
    if (interval_count < 2) return;

    // Use last few intervals for average
    uint8_t samples = (interval_count < HISTORY_SIZE) ? interval_count : HISTORY_SIZE;
    uint32_t total = 0;

    for (uint8_t i = 0; i < samples; i++) {
        total += beat_intervals[i];
    }

    uint32_t avg_interval = total / samples;
    current_bpm = (60000 / avg_interval);
}

bool pulse_sensor_beat_detected(void) {
    if (new_beat) {
        new_beat = false;
        return true;
    }
    return false;
}

uint16_t pulse_sensor_get_bpm(void) {
    return current_bpm;
}

// Simplified quality check - just return true for now
bool pulse_sensor_bpm_valid(void) {
    return (current_bpm >= 40 && current_bpm <= 180 && interval_count >= 2);
}

uint8_t get_signal_quality(void) {
    return interval_count >= 2 ? 80 : 20;  // Simple binary quality
}
