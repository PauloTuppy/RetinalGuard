# RetinalGuard - Guia Rápido: Usar Código Corrigido
## De Correções para GitHub em 5 Passos

---

## 🚀 5 Passos Rápidos

### PASSO 1: Clonar repositório base

```bash
# Criar novo repo
mkdir retinalguard-tuya-t5
cd retinalguard-tuya-t5
git init
git remote add origin https://github.com/yourname/retinalguard-tuya-t5.git
```

### PASSO 2: Copiar estrutura do arquivo [84]

Copie os seguintes arquivos do documento "Código Corrigido" (artifact 84):

```
firmware/
├── main.c              ← Use versão CORRIGIDA (tem timestamps, arrays)
├── tflite_inference.c  ← Use versão CORRIGIDA (tem output_data[0])
├── sensor_fusion.h     ← Novo header com protótipos
├── camera_capture.c    ← Stub simulado (abaixo)
├── sensor_fusion.c     ← Stub simulado (abaixo)
├── mqtt_handler.c      ← Stub simulado (abaixo)
├── ui_display.c        ← Stub simulado (abaixo)
├── Makefile            ← CORRIGIDO (compilável)
└── tuya_iot_config.h   ← Configurações

models/
├── fundus_cvd_mobilenetv3.tflite  ← Dummy ou link
├── model_training.ipynb            ← Notebook
└── quantization_script.py          ← Script

cloud_agent/
├── report_generation.py   ← CORRIGIDO (executável)
├── tuya_api_wrapper.py    ← Stub
├── requirements.txt       ← Corrigido
└── example_integration.py ← Demo

docs/
├── ARCHITECTURE.md
├── HARDWARE_SETUP.md
├── FIRMWARE_BUILD.md
└── DEPLOYMENT_GUIDE.md
```

### PASSO 3: Criar stubs para compilação

Para os arquivos que você ainda não tem implementados, crie stubs compiláveis:

#### `firmware/camera_capture.c` (stub)

```c
#include "camera_capture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int camera_init_dvp(void) {
    printf("[CAMERA] DVP initialization (stub)\n");
    return 1;
}

uint8_t* camera_capture_fundus(uint8_t* buffer, int width, int height) {
    printf("[CAMERA] Capturing fundus image %dx%d (stub)\n", width, height);
    
    if (!buffer) return NULL;
    
    // Preencher com dados simulados (padrão xadrez para teste)
    for (int i = 0; i < width * height * 3; i++) {
        buffer[i] = (i % 256);
    }
    
    return buffer;
}
```

#### `firmware/sensor_fusion.c` (stub)

```c
#include "sensor_fusion.h"
#include <stdio.h>

int sensor_init_ppg(void) {
    printf("[SENSOR] PPG initialization (stub)\n");
    return 1;
}

int sensor_init_temperature(void) {
    printf("[SENSOR] Temperature initialization (stub)\n");
    return 1;
}

int sensor_read_all(SensorReadings* out) {
    printf("[SENSOR] Reading all sensors (stub)\n");
    
    if (!out) return 0;
    
    out->heart_rate = 75;
    out->blood_oxygen = 97.5f;
    out->temperature = 36.8f;
    out->timestamp_ms = 0;
    
    return 1;
}

float sensor_fusion_calculate_risk(float retinal_risk, 
                                  const SensorReadings* vitals) {
    printf("[SENSOR] Fusing retinal (%.2f) + vitals\n", retinal_risk);
    
    // Simples: 60% retinal, 40% vitals
    float vital_risk = 0.3f;  // Exemplo
    float final = 0.6f * retinal_risk + 0.4f * vital_risk;
    
    return final;
}
```

#### `firmware/mqtt_handler.c` (stub)

```c
#include "mqtt_handler.h"
#include <stdio.h>

int mqtt_connect(const char* device_id) {
    printf("[MQTT] Connecting to Tuya Cloud as %s (stub)\n", device_id);
    return 1;
}

int mqtt_send_screening_result(const void* result) {
    printf("[MQTT] Sending screening result (stub)\n");
    return 1;
}
```

#### `firmware/ui_display.c` (stub)

```c
#include "ui_display.h"
#include <stdio.h>

int ui_init(void) {
    printf("[UI] Display initialization (stub)\n");
    return 1;
}

int ui_display_result(float risk, const void* vitals, const char* level) {
    printf("[UI] Displaying result: %s (%.1f%%)\n", level, risk * 100);
    return 1;
}
```

#### `firmware/camera_capture.h` (header)

```c
#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <stdint.h>

int camera_init_dvp(void);
uint8_t* camera_capture_fundus(uint8_t* buffer, int width, int height);

#endif
```

#### `firmware/mqtt_handler.h` (header)

```c
#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

int mqtt_connect(const char* device_id);
int mqtt_send_screening_result(const void* result);

#endif
```

#### `firmware/ui_display.h` (header)

```c
#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

int ui_init(void);
int ui_display_result(float risk, const void* vitals, const char* level);

#endif
```

### PASSO 4: Testar compilação

```bash
cd firmware

# Installar Tuya SDK (or mock for demo)
# export TUYA_SDK_PATH=/opt/tuya_sdk
# export TENSORFLOW_LITE_PATH=/opt/tensorflow_lite

# Compilar
make clean
make

# Resultado esperado:
# ✓ Compiled: firmware/main.c
# ✓ Compiled: firmware/camera_capture.c
# ✓ Compiled: firmware/tflite_inference.c
# ✓ Compiled: firmware/sensor_fusion.c
# ✓ Compiled: firmware/mqtt_handler.c
# ✓ Compiled: firmware/ui_display.c
# ✓ Linked: bin/retinalguard.elf
# ✓ Size: 512K (aproximado)
```

### PASSO 5: Commit e Push

```bash
cd ..

# Criar .gitignore
cat > .gitignore << EOF
build/
bin/
*.o
*.elf
*.pyc
__pycache__/
.venv/
.env
EOF

# Criar README.md (use versão simplificada do arquivo [82])
cat > README.md << EOF
# RetinalGuard - AI Health Scanner on Tuya T5

Open-source cardiovascular health screening device combining edge AI (TensorFlow Lite) 
with Tuya T5 hardware and cloud AI agents.

## Quick Start

\`\`\`bash
cd firmware && make
python cloud_agent/report_generation.py
\`\`\`

## Hardware

- Tuya T5 AI-Core
- GC2145 2MP camera (DVP)
- MAX30102 PPG sensor
- DHT22 temperature sensor

## License

MIT

#TuyaOpen #TuyaAI #HackWithTuya
EOF

# Commitar
git add -A
git commit -m "Initial working prototype - FP32 quantization, stubs for compilation"
git push origin main
```

---

## ✅ CHECKLIST DE COMPILAÇÃO

- [ ] firmware/main.c copiado (versão corrigida do arquivo [84])
- [ ] firmware/tflite_inference.c copiado (com output_data[0])
- [ ] firmware/sensor_fusion.h criado (protótipos)
- [ ] firmware/Makefile copiado (com paths corretos)
- [ ] Stubs criados: camera_capture.c, mqtt_handler.c, ui_display.c, sensor_fusion.c
- [ ] Headers criados: camera_capture.h, mqtt_handler.h, ui_display.h, sensor_fusion.h
- [ ] Makefile editado: paths para Tuya SDK e TensorFlow Lite
- [ ] Compilação bem-sucedida: `make` gera bin/retinalguard.elf
- [ ] .gitignore criado
- [ ] README.md criado
- [ ] Git init, add, commit, push executado

---

## 🐛 TROUBLESHOOTING

### Erro: "undefined reference to `TfLiteModelCreateFromFile`"

**Solução:** TensorFlow Lite path não configurado

```bash
export TENSORFLOW_LITE_PATH=/opt/tensorflow_lite
make clean && make
```

### Erro: "undefined reference to `tuya_cloud_service_init`"

**Solução:** Tuya SDK path não configurado

```bash
export TUYA_SDK_PATH=/opt/tuya_sdk
make clean && make
```

### Compilação OK, mas erro no `make upload`

**Solução:** Para demo, ignore upload (você fará isso na CES se passar)

```bash
# Apenas compile
make

# Não execute make upload ainda
```

### Erro em `strncpy(result.risk_level, ...)`

**Solução:** Verifique que `sensor_fusion.h` tem tamanhos corretos

```c
// ✅ CORRETO
char risk_level[16];
strncpy(result.risk_level, "HIGH", sizeof(result.risk_level) - 1);
```

---

## 📊 ESTRUTURA FINAL DO REPO

```
retinalguard-tuya-t5/
├── README.md
├── LICENSE (MIT)
├── .gitignore
│
├── firmware/
│   ├── main.c (CORRIGIDO)
│   ├── tflite_inference.c (CORRIGIDO)
│   ├── camera_capture.c (STUB)
│   ├── camera_capture.h
│   ├── sensor_fusion.c (STUB)
│   ├── sensor_fusion.h
│   ├── mqtt_handler.c (STUB)
│   ├── mqtt_handler.h
│   ├── ui_display.c (STUB)
│   ├── ui_display.h
│   ├── tuya_iot_config.h
│   ├── Makefile (CORRIGIDO)
│   └── .gitkeep
│
├── models/
│   ├── fundus_cvd_mobilenetv3.tflite (or link)
│   ├── model_training.ipynb
│   └── quantization_script.py
│
├── cloud_agent/
│   ├── report_generation.py (CORRIGIDO)
│   ├── tuya_api_wrapper.py
│   ├── requirements.txt (CORRIGIDO)
│   └── example_integration.py
│
├── docs/
│   ├── ARCHITECTURE.md
│   ├── HARDWARE_SETUP.md
│   ├── FIRMWARE_BUILD.md
│   └── DEPLOYMENT_GUIDE.md
│
└── hardware/
    ├── schematic.kicad_sch (or notes)
    ├── BOM.csv
    └── README.md
```

---

## 🎯 PRÓXIMA ETAPA

Após fazer push para GitHub:

1. Copie o link do repo: `https://github.com/yourname/retinalguard-tuya-t5`
2. Teste a compilação em CI (GitHub Actions - optional)
3. Use esse link no formulário do hackathon (Campo 1)
4. Continue com documentação e demo video (do arquivo [83])

---

## 📝 REFERÊNCIA RÁPIDA - ARQUIVOS CORRIGIDOS

| Arquivo | Correção | Fonte |
|---------|----------|--------|
| main.c | Arrays char, timestamps, time handling | [84] |
| tflite_inference.c | output_data[0], gettimeofday() | [84] |
| Makefile | Paths Tuya SDK/TFLite | [84] |
| report_generation.py | Error handling, logging | [84] |
| sensor_fusion.h | Protótipos corretos | [84] |

Todos incluídos no documento **"Código Corrigido"** (artifact 84).

---

**RetinalGuard GitHub Setup - Ready to Go! ✅**