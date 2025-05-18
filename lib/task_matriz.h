#ifndef TASK_MATRIZ_H
#define TASK_MATRIZ_H

#include "hardware/pio.h"
#include "hardware/clocks.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
#define BRILHO 0.05

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = (uint8_t)(r * BRILHO);
  leds[index].G = (uint8_t)(g * BRILHO);
  leds[index].B = (uint8_t)(b * BRILHO);
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// Modificado do github: https://github.com/BitDogLab/BitDogLab-C/tree/main/neopixel_pio
// Função para converter a posição do matriz para uma posição do vetor.
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

void print_sprite(int matriz[5][5][3]) {
  // Desenhando Sprite contido na matriz.c
  for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
}

// TASK DA MATRIZ RGB
void vMatrizTask(void *params) {
    npInit(LED_PIN);
    bool piscar = true;
    sensor_data_t dados;

    // Símbolos fixos
    const int exclama[] = {23, 18, 13, 3}; // Alerta geral

    const int gota[] = {                     // Alerta por chuva
                22,
            18, 17, 16,
        14, 13, 12, 11, 10,
          9, 8, 7, 6, 5,
            3, 2, 1
    };

    const int copo[][3] = {                  // Alerta por nível de água
        {3, 2, 1},
        {8, 7, 6},
        {13, 12, 11}
    };
    const int copo_frames = 3;
    static int frame_copo = 0;

    while (1) {
        if (xQueueReceive(xQueueSensorData, &dados, portMAX_DELAY) == pdTRUE) {
            npClear();

            if (dados.alerta) {
                if (dados.nivel_agua_pct >= 70 && dados.volume_chuva_pct >= 80) {
                    // ALERTA GERAL: Exclamação piscando (250ms)
                    if (piscar) {
                        for (int i = 0; i < sizeof(exclama) / sizeof(int); i++)
                            npSetLED(exclama[i], 255, 0, 0); // Vermelho
                    }
                    piscar = !piscar;
                }
                else if (dados.nivel_agua_pct >= 70) {
                    // ALERTA por Nível: copo enchendo (200ms)
                    for (int f = 0; f <= frame_copo; f++) {
                        for (int i = 0; i < 3; i++) {
                            npSetLED(copo[f][i], 0, 255, 255); // Ciano
                        }
                    }
                    frame_copo = (frame_copo + 1) % (copo_frames + 1);
                }
                else if (dados.volume_chuva_pct >= 80) {
                    // ALERTA por Chuva: gota piscando (150ms)
                    if (piscar) {
                        for (int i = 0; i < sizeof(gota) / sizeof(int); i++)
                            npSetLED(gota[i], 0, 0, 255); // Azul
                    }
                    piscar = !piscar;
                }
            } else {
                // Sem alerta: tudo apagado
                frame_copo = 0;
                piscar = true;
                
            }
            npWrite();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}


#endif