#ifndef TASK_LEDRGB_H
#define TASK_LEDRGB_H

#define LED_VERDE 11
#define LED_VERMELHO 13

void vLedRGBTask(void *params) {
    gpio_init(LED_VERDE);
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

    sensor_data_t dados;

    while (true) {
        if (xQueueReceive(xQueueSensorData, &dados, portMAX_DELAY) == pdTRUE) {
            if (dados.alerta) {
                gpio_put(LED_VERDE, 0); gpio_put(LED_VERMELHO, 1);   // Liga vermelho
            } else {
                gpio_put(LED_VERMELHO, 0); gpio_put(LED_VERDE, 1);      // Liga verde
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


#endif