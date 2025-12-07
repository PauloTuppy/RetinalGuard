#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <stdint.h>

int camera_init_dvp(void);
uint8_t* camera_capture_fundus(int width, int height);

#endif
