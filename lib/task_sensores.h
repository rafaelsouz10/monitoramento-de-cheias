#ifndef TASK_SENSORES_H
#define TASK_SENSORES_H

void vSensoresTask(void *params) {
    #define ADC_AGUA 26   // eixo X
    #define ADC_CHUVA 27  // eixo Y

    adc_init();
    adc_gpio_init(ADC_AGUA);   // GPIO 26 = ADC0
    adc_gpio_init(ADC_CHUVA);  // GPIO 27 = ADC1

    sensor_data_t dados;

    while (true) {
        // Lê nível da água (X)
        adc_select_input(1);  // ADC0
        uint16_t valor_agua = adc_read();

        // Lê volume de chuva (Y)
        adc_select_input(0);  // ADC1
        uint16_t valor_chuva = adc_read();

        // Converte para porcentagem
        dados.nivel_agua_pct = (valor_agua * 100) / 4095;
        dados.volume_chuva_pct = (valor_chuva * 100) / 4095;

        // Define alerta
        dados.alerta = (dados.nivel_agua_pct >= 70 || dados.volume_chuva_pct >= 80);

        // DEBUG: imprime no terminal
        printf("Água: %02u%% | Chuva: %02u%% | Alerta: %s\n", dados.nivel_agua_pct, dados.volume_chuva_pct, dados.alerta ? "SIM" : "NAO");

        // Envia para fila
        xQueueSend(xQueueSensorData, &dados, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(300));
    }
}


#endif