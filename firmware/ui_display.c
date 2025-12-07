#include "ui_display.h"
#include <stdio.h>

void ui_init(void) {
    printf("    [MOCK] Initializing OLED display...\n");
}

void ui_display_result(float risk, SensorReadings vitals, const char* level) {
    printf("    [MOCK] Displaying: Risk %.2f (%s)\n", risk, level);
}
