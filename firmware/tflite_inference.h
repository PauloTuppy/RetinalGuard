#ifndef TFLITE_INFERENCE_H
#define TFLITE_INFERENCE_H

#include <stdint.h>

int tflite_init(const char* model_path);
float tflite_inference_fundus(uint8_t* fundus_image);
float estimate_systolic_bp(uint8_t* fundus_image);
void tflite_cleanup(void);

#endif
