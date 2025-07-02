#include "DHT11.hpp"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_cortex.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h743xx.h"
#include <stdint.h>

// À adapter selon ta config GPIO
#define DHT_PORT GPIOA
#define DHT_PIN  GPIO_PIN_1

extern TIM_HandleTypeDef htim1; // Timer configuré en µs

void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

void DHT11_Start() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure en sortie
    GPIO_InitStruct.Pin = DHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_RESET);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_SET);
    delay_us(20);

    // Configure en entrée
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);
}

uint8_t DHT11_CheckResponse() {
    delay_us(40);
    if (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) {
        delay_us(80);
        if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) {
            delay_us(80);
            return 1;
        }
    }
    return 0;
}

uint8_t DHT11_ReadByte() {
    uint8_t i = 0;
    for (int j = 0; j < 8; j++) {
        while (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)); // wait for HIGH
        delay_us(40);
        if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))
            i |= (1 << (7 - j));
        while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)); // wait for LOW
    }
    return i;
}

int DHT11_ReadTemperature() {
    uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2, checksum;
    int temperature = 10; // valeur par défaut en cas d'erreur

    DHT11_Start();
    if (DHT11_CheckResponse()) {
        Rh_byte1 = DHT11_ReadByte();
        Rh_byte2 = DHT11_ReadByte();
        Temp_byte1 = DHT11_ReadByte();
        Temp_byte2 = DHT11_ReadByte();
        checksum = DHT11_ReadByte();

        if (checksum == (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2)) {
            temperature = Temp_byte1;
        }
    }

    return temperature;
}
