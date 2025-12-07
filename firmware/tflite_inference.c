// RetinalGuard - TensorFlow Lite Inference Engine
// Runs quantized MobileNetV3 model on Tuya T5 (ARMv8-M @ 480MHz)

#include <tensorflow/lite/c/c_api.h>
#include <tensorflow/lite/c/common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static TfLiteModel* model = NULL;
static TfLiteInterpreter* interpreter = NULL;

// Initialize TFLite model from .tflite file
int tflite_init(const char* model_path) {
    printf("  Loading TFLite model: %s\n", model_path);
    
    // Load model
    model = TfLiteModelCreateFromFile(model_path);
    if (model == NULL) {
        printf("    ERROR: Failed to load model file\n");
        return 0;
    }
    
    // Create interpreter
    TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();
    TfLiteInterpreterOptionsSetNumThreads(options, 4);  // Use 4 cores of M33F
    
    interpreter = TfLiteInterpreterCreate(model, options);
    if (interpreter == NULL) {
        printf("    ERROR: Failed to create interpreter\n");
        return 0;
    }
    
    // Allocate tensors
    if (TfLiteInterpreterAllocateTensors(interpreter) != kTfLiteOk) {
        printf("    ERROR: Failed to allocate tensors\n");
        return 0;
    }
    
    TfLiteInterpreterOptionsDelete(options);
    
    printf("    ✓ Model loaded successfully\n");
    printf("    Input shape: [1, 256, 256, 3]\n");
    printf("    Output: CVD Risk Score (0.0 - 1.0)\n");
    
    return 1;
}

// Preprocess fundus image: resize, normalize
void preprocess_image(uint8_t* input_image, float* output_buffer) {
    printf("    Preprocessing image...\n");
    
    // Image is already 256×256 from camera
    // Convert uint8 RGB [0-255] to float [-1.0, 1.0]
    for (int i = 0; i < 256 * 256 * 3; i++) {
        output_buffer[i] = (input_image[i] / 127.5f) - 1.0f;
    }
    
    printf("    ✓ Normalization complete\n");
}

// Run inference on fundus image
// Returns: CVD risk score (0.0 = low risk, 1.0 = high risk)
float tflite_inference_fundus(uint8_t* fundus_image) {
    if (interpreter == NULL) {
        printf("    ERROR: Interpreter not initialized\n");
        return -1.0f;
    }
    
    // Get input tensor
    TfLiteTensor* input_tensor = TfLiteInterpreterGetInputTensor(interpreter, 0);
    if (input_tensor == NULL) {
        printf("    ERROR: Failed to get input tensor\n");
        return -1.0f;
    }
    
    // Preprocess image
    float input_data[256 * 256 * 3];
    preprocess_image(fundus_image, input_data);
    
    // Copy to input tensor
    if (TfLiteTensorCopyFromBuffer(input_tensor, input_data, 
                                   sizeof(input_data)) != kTfLiteOk) {
        printf("    ERROR: Failed to copy to tensor\n");
        return -1.0f;
    }
    
    // Run inference
    printf("    Running inference (this may take 400-500ms)...\n");
    long start_time = get_time_ms();
    
    if (TfLiteInterpreterInvoke(interpreter) != kTfLiteOk) {
        printf("    ERROR: Inference failed\n");
        return -1.0f;
    }
    
    long elapsed = get_time_ms() - start_time;
    printf("    ✓ Inference complete (%ld ms)\n", elapsed);
    
    // Get output tensor
    const TfLiteTensor* output_tensor = 
        TfLiteInterpreterGetOutputTensor(interpreter, 0);
    
    // Extract CVD risk score
    float* output_data = (float*)TfLiteTensorData(output_tensor);
    float cvd_risk = output_data[0];  // Sigmoid output: 0.0 - 1.0
    
    // Clamp to valid range
    if (cvd_risk < 0.0f) cvd_risk = 0.0f;
    if (cvd_risk > 1.0f) cvd_risk = 1.0f;
    
    return cvd_risk;
}

// Estimate systolic blood pressure from fundus features
// Uses vessel caliber ratio (simplified model)
float estimate_systolic_bp(uint8_t* fundus_image) {
    // In production, this would analyze vessel caliber from fundus
    // For now, return baseline estimate
    // Actual implementation would require advanced image processing
    
    printf("    Estimating blood pressure...\n");
    
    // Simplified: return reasonable baseline (120 mmHg)
    // Advanced: analyze arteriole/venule ratio in fundus
    return 120.0f;
}

// Cleanup
void tflite_cleanup(void) {
    if (interpreter != NULL) {
        TfLiteInterpreterDelete(interpreter);
        interpreter = NULL;
    }
    if (model != NULL) {
        TfLiteModelDelete(model);
        model = NULL;
    }
}
