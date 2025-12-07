# RetinalGuard Architecture

See `assets/architecture_diagram.txt` for the visual representation.

## Components
1. **Tuya T5 Edge Device**: Runs the firmware, TFLite inference, and sensor fusion.
2. **Sensors**: Camera (GC2145), PPG (MAX30102), Temp (DHT22).
3. **Cloud Agent**: Tuya Cloud processing for report generation.
