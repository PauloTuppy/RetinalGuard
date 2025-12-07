#include <stdio.h>
#include "../firmware/camera_capture.h"

int main() {
    printf("Testing Camera Module...\n");
    if (camera_init_dvp()) {
        printf("PASS: Camera Init\n");
    } else {
        printf("FAIL: Camera Init\n");
        return 1;
    }
    
    uint8_t* frame = camera_capture_fundus(256, 256);
    if (frame) {
        printf("PASS: Frame Capture\n");
        free(frame);
    } else {
        printf("FAIL: Frame Capture\n");
        return 1;
    }
    
    return 0;
}
