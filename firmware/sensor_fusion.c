#include "sensor_fusion.h"
#include <stdio.h>

void sensor_init_ppg(void) {
    printf("    [MOCK] Initializing MAX30102 PPG sensor...\n");
}

void sensor_init_temperature(void) {
    printf("    [MOCK] Initializing DHT22 temperature sensor...\n");
}

SensorReadings sensor_read_all(void) {
    SensorReadings readings;
    readings.heart_rate = 75; // Mock
    readings.blood_oxygen = 98.5; // Mock
    readings.temperature = 36.6; // Mock
    return readings;
}

float sensor_fusion_calculate_risk(float cvd_risk_score, SensorReadings vitals) {
    // Simple mock fusion: just weight them
    // Real logic would be complex
    return cvd_risk_score * 0.7 + (vitals.heart_rate > 90 ? 0.1 : 0.0);
}
