// RetinalGuard - Main Firmware for Tuya T5 AI-Core
// Health screening device using retinal image analysis + vital signs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tuya_cloud_service.h"
#include "tuya_iot_sdk.h"
#include "gw_wifi.h"
#include "camera_capture.h"
#include "tflite_inference.h"
#include "sensor_fusion.h"
#include "mqtt_handler.h"
#include "ui_display.h"

// Configuration
#define DEVICE_ID "your_tuya_device_id"
#define TUYA_REGION "us"
#define INFERENCE_INTERVAL 5000  // 5 seconds between screenings

// Global variables
typedef struct {
    float cvd_risk_score;
    float blood_pressure_systolic;
    float blood_oxygen;
    int heart_rate;
    float temperature;
    char timestamp[32];
    char risk_level[16];
} HealthScreeningResult;

HealthScreeningResult latest_result;

// ===== MAIN APPLICATION FLOW =====

void health_screening_task(void) {
    printf("[RetinalGuard] Starting health screening...\n");
    
    // Step 1: Capture retinal image from GC2145 camera
    printf("  [STEP 1] Capturing retinal fundus image...\n");
    uint8_t* fundus_image = camera_capture_fundus(256, 256);
    
    if (!fundus_image) {
        printf("    ERROR: Failed to capture image\n");
        return;
    }
    printf("    ✓ Image captured (256×256 RGB)\n");
    
    // Step 2: Run TensorFlow Lite inference on T5 edge device
    printf("  [STEP 2] Running TFLite inference on T5...\n");
    float cvd_risk_score = tflite_inference_fundus(fundus_image);
    printf("    ✓ CVD Risk Score: %.2f (%.1f%%)\n", cvd_risk_score, cvd_risk_score * 100);
    
    // Step 3: Capture vital signs from sensors
    printf("  [STEP 3] Reading vital signs sensors...\n");
    SensorReadings vitals = sensor_read_all();
    printf("    ✓ Heart Rate: %d bpm\n", vitals.heart_rate);
    printf("    ✓ O2 Saturation: %.1f%%\n", vitals.blood_oxygen);
    printf("    ✓ Temperature: %.1f°C\n", vitals.temperature);
    
    // Step 4: Fuse retinal + vital signs for final score
    printf("  [STEP 4] Sensor fusion...\n");
    float final_risk = sensor_fusion_calculate_risk(cvd_risk_score, vitals);
    printf("    ✓ Final Risk Score: %.2f (%.1f%%)\n", final_risk, final_risk * 100);
    
    // Step 5: Determine risk level and recommendation
    printf("  [STEP 5] Risk classification...\n");
    const char* risk_level = (final_risk > 0.7) ? "HIGH" : 
                             (final_risk > 0.4) ? "MODERATE" : "LOW";
    printf("    ✓ Risk Level: %s\n", risk_level);
    
    // Step 6: Display result on UI (if available)
    printf("  [STEP 6] Updating display...\n");
    ui_display_result(final_risk, vitals, risk_level);
    printf("    ✓ Display updated\n");
    
    // Step 7: Send results to Tuya Cloud for AI Agent processing
    printf("  [STEP 7] Sending to Tuya Cloud...\n");
    HealthScreeningResult result = {
        .cvd_risk_score = final_risk,
        .blood_pressure_systolic = estimate_systolic_bp(fundus_image),
        .blood_oxygen = vitals.blood_oxygen,
        .heart_rate = vitals.heart_rate,
        .temperature = vitals.temperature
    };
    strcpy(result.risk_level, risk_level);
    snprintf(result.timestamp, sizeof(result.timestamp), "%ld", time(NULL));
    
    mqtt_send_screening_result(&result);
    printf("    ✓ Results sent to cloud\n");
    
    // Store locally for offline operation
    latest_result = result;
    
    printf("[RetinalGuard] Screening complete. Risk: %s (%.1f%%)\n\n", 
           risk_level, final_risk * 100);
    
    free(fundus_image);
}

// ===== INITIALIZATION =====

void retinalguard_init(void) {
    printf("=== RetinalGuard v1.0 Initialization ===\n");
    
    // Initialize camera
    printf("[INIT] Camera module...\n");
    if (!camera_init_dvp()) {
        printf("  ERROR: Camera initialization failed\n");
        return;
    }
    printf("  ✓ GC2145 camera ready\n");
    
    // Initialize TFLite interpreter
    printf("[INIT] TensorFlow Lite...\n");
    if (!tflite_init("fundus_cvd_mobilenetv3.tflite")) {
        printf("  ERROR: TFLite model load failed\n");
        return;
    }
    printf("  ✓ Model loaded (MobileNetV3-Lite)\n");
    
    // Initialize sensors
    printf("[INIT] Biometric sensors...\n");
    sensor_init_ppg();      // MAX30102
    sensor_init_temperature(); // DHT22
    printf("  ✓ PPG + Temperature sensors ready\n");
    
    // Initialize MQTT for cloud
    printf("[INIT] MQTT/Cloud connectivity...\n");
    if (!mqtt_connect(DEVICE_ID)) {
        printf("  ERROR: MQTT connection failed\n");
        return;
    }
    printf("  ✓ Connected to Tuya Cloud\n");
    
    // Initialize display (optional)
    printf("[INIT] Display module...\n");
    ui_init();
    printf("  ✓ Display ready\n");
    
    printf("\n✅ RetinalGuard initialized successfully!\n");
    printf("Ready to perform health screenings...\n\n");
}

// ===== MAIN LOOP =====

int main(void) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   RetinalGuard - AI Health Scanner    ║\n");
    printf("║   Tuya T5 AI-Core Edition             ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    // Initialize hardware
    retinalguard_init();
    
    // Main loop: perform screening every INFERENCE_INTERVAL ms
    printf("Entering main loop. Press Ctrl+C to stop.\n\n");
    
    while (1) {
        health_screening_task();
        sleep(INFERENCE_INTERVAL / 1000);
    }
    
    return 0;
}

// ===== EVENT HANDLERS =====

// Called when cloud sends command (e.g., "start screening")
void mqtt_command_handler(const char* command, const char* param) {
    printf("[CLOUD COMMAND] %s (param: %s)\n", command, param);
    
    if (strcmp(command, "start_screening") == 0) {
        health_screening_task();
    } 
    else if (strcmp(command, "get_status") == 0) {
        printf("  Status: READY\n");
        printf("  Last Result: CVD Risk %.1f%%\n", latest_result.cvd_risk_score * 100);
    }
}

// Called when WiFi connects
void wifi_event_handler(const char* event) {
    printf("[WIFI EVENT] %s\n", event);
    if (strcmp(event, "connected") == 0) {
        mqtt_connect(DEVICE_ID);
    }
}
