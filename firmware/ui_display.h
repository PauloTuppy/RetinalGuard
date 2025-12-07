#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include "sensor_fusion.h"

void ui_init(void);
void ui_display_result(float risk, SensorReadings vitals, const char* level);

#endif
