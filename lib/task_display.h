#ifndef TASK_DISPLAY_H
#define TASK_DISPLAY_H

#include "hardware/i2c.h"
#include "ssd1306/ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

bool cor = true;    // Define a cor do display (preto/branco)
ssd1306_t ssd;    // Inicializa a estrutura do display

void display_init(){
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); //Inicializa o display
    ssd1306_config(&ssd); //Configura o display
    ssd1306_send_data(&ssd); //Envia os dados para o display

    //O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// TASK DO DISPLAY
void vDisplayTask(){
    display_init();

    sensor_data_t dados;

    char linha1[32];
    char linha2[32];
    char linha3[32];

    while (true) {

        // Recebe da fila e armazena em 'dados' assim que houver novo valor disponível
        if (xQueueReceive(xQueueSensorData, &dados, portMAX_DELAY) == pdTRUE) {
            // Formata strings
            snprintf(linha1, sizeof(linha1), "Nivel Agua: %2u%%", dados.nivel_agua_pct);
            snprintf(linha2, sizeof(linha2), "Chuva: %2u%%", dados.volume_chuva_pct);
            snprintf(linha3, sizeof(linha3), "Status: %s", dados.alerta ? "ALERTA" : "NORMAL");
            
            ssd1306_fill(&ssd, 0); // limpa o display
            ssd1306_draw_string(&ssd, linha1, 0, 10);
            ssd1306_draw_string(&ssd, linha2, 0, 25);
            ssd1306_draw_string(&ssd, linha3, 0, 45);
            ssd1306_send_data(&ssd); // Envia os dados para o display
        }
    }
}

#endif