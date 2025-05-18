#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// --- Tipo de dado da fila ---
typedef struct {
    uint16_t nivel_agua;       // De 0 a 4095 (valor bruto do ADC)
    uint16_t volume_chuva;
    bool alerta;               // true se estiver em modo alerta
} sensor_data_t;

// --- Fila global ---
QueueHandle_t xQueueSensorData;

// --- Protótipos das tasks ---
void vSensorTask(void *params);
// void vAnaliseTask(void *params);
// void vDisplayTask(void *params);
// void vLedRGBTask(void *params);
// void vBuzzerTask(void *params);
// void vMatrizTask(void *params);

int main() {
    stdio_init_all();

    // Criação da fila para troca de dados entre tasks
    xQueueSensorData = xQueueCreate(5, sizeof(sensor_data_t));
    if (xQueueSensorData == NULL) {
        printf("Erro ao criar fila!\n");
        while (true); // trava aqui se erro
    }

    // Criação das tarefas (por enquanto só a leitura e display)
    xTaskCreate(vSensorTask, "Sensor", 256, NULL, 1, NULL);
    // xTaskCreate(vAnaliseTask, "Analise", 256, NULL, 1, NULL);
    // xTaskCreate(vDisplayTask, "Display", 512, NULL, 1, NULL);
    // xTaskCreate(vLedRGBTask, "RGB", 256, NULL, 1, NULL);
    // xTaskCreate(vBuzzerTask, "Buzzer", 256, NULL, 1, NULL);
    // xTaskCreate(vMatrizTask, "Matriz", 256, NULL, 1, NULL);

    vTaskStartScheduler();
    while (true); // nunca deve chegar aqui
}

void vSensorTask(void *params) {
    #define ADC_AGUA 26
    #define ADC_CHUVA 27
    
    adc_init();
    adc_gpio_init(ADC_AGUA); // ADC0 - eixo X
    adc_gpio_init(ADC_CHUVA); // ADC1 - eixo Y

    sensor_data_t dados;

    while (true) {
        // Lê volume de chuva (Y)
        adc_select_input(0);  // GPIO 27 = ADC1
        dados.volume_chuva = adc_read();

        // Lê nível da água (X)
        adc_select_input(1);  // GPIO 26 = ADC0
        dados.nivel_agua = adc_read();

        // Por enquanto, deixa o alerta falso (será ajustado na vAnaliseTask)
        dados.alerta = false;

        // Envia struct para fila
        xQueueSend(xQueueSensorData, &dados, portMAX_DELAY);

        // Aguarda 500ms antes da próxima leitura
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
