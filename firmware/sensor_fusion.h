#ifndef SENSOR_FUSION_H
#define SENSOR_FUSION_H

typedef struct {
    int heart_rate;
    float blood_oxygen;
    float temperature;
} SensorReadings;

void sensor_init_ppg(void);
void sensor_init_temperature(void);
SensorReadings sensor_read_all(void);
float sensor_fusion_calculate_risk(float cvd_risk_score, SensorReadings vitals);

#endif
