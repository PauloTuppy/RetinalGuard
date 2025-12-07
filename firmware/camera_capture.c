#include "camera_capture.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize camera module (GC2145 via DVP)
int camera_init_dvp(void) {
    printf("    [MOCK] Initializing GC2145 camera via DVP interface...\n");
    // In real hardware: i2c_write_reg(ADDR, REG, VAL)...
    return 1; // Success
}

// Capture a frame
// Returns pointer to RGB buffer
uint8_t* camera_capture_fundus(int width, int height) {
    printf("    [MOCK] Capturing frame %dx%d...\n", width, height);
    
    size_t size = width * height * 3;
    uint8_t* buffer = (uint8_t*)malloc(size);
    if (!buffer) return NULL;
    
    // Fill with dummy data
    for (size_t i = 0; i < size; i++) {
        buffer[i] = (uint8_t)(i % 255);
    }
    
    return buffer;
}
