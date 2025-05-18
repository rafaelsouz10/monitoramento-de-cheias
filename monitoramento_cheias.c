#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// --- Tipo de dado da fila ---
typedef struct {
    uint8_t nivel_agua_pct;      // 0 a 100 (%)
    uint8_t volume_chuva_pct;    // 0 a 100 (%)
    bool alerta;               // true se estiver em modo alerta
} sensor_data_t;

// --- Fila global ---
QueueHandle_t xQueueSensorData;

// --- Libs Tasks ---
#include "lib/bootsel_btn.h"
#include "lib/task_sensores.h"
#include "lib/task_display.h"
#include "lib/task_ledRGB.h"

int main() {
    bootsel_btn_callback(); // Para ser utilizado o modo BOOTSEL com botão B

    stdio_init_all();
    sleep_ms(1000);
    
    // Criação da fila
    xQueueSensorData = xQueueCreate(5, sizeof(sensor_data_t));
    if (xQueueSensorData == NULL) {
        printf("Erro ao criar fila!\n");
        while (true); // trava aqui se erro
    }

    // Criação das Tasks
    xTaskCreate(vSensoresTask, "Sensores", 256, NULL, 1, NULL);
    xTaskCreate(vDisplayTask, "Display", 512, NULL, 1, NULL);
    xTaskCreate(vLedRGBTask, "RGB", 256, NULL, 1, NULL);


    vTaskStartScheduler();
}

