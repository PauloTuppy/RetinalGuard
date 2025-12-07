CC=gcc
CFLAGS=-I. -Wall
# Note: TFLite and other Tuya libs would need real paths in a real build
# This makefile builds the mock version
LIBS=-lm

OBJS=main.o camera_capture.o tflite_inference.o sensor_fusion.o mqtt_handler.o ui_display.o

all: retinal_guard

retinal_guard: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

build: all

clean:
	rm -f *.o retinal_guard

upload:
	@echo "Mock: Uploading to Tuya T5..."
