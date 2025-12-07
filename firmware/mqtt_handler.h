#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <stdbool.h>

bool mqtt_connect(const char* device_id);
void mqtt_send_screening_result(void* result);

#endif
