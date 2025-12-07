#include "mqtt_handler.h"
#include <stdio.h>

bool mqtt_connect(const char* device_id) {
    printf("    [MOCK] Connecting to MQTT broker as %s...\n", device_id);
    return true;
}

void mqtt_send_screening_result(void* result) {
    printf("    [MOCK] Sending screening result payload to topic 'tuya/feeds'...\n");
    // In real code, we would cast result to HealthScreeningResult* and serialize to JSON
}
