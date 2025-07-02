#ifndef SAMPLE_TOP_HPP
#define SAMPLE_TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"
#include "temperature.hpp"
#include "Digitalout.hpp"
#include "stm32h7xx_hal_rcc.h"
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_hal_uart.h"


using namespace cadmium;

struct top_coupled : public Coupled {
    top_coupled(const std::string& id) : Coupled(id) {
         __HAL_RCC_GPIOB_CLK_ENABLE();
           static GPIO_InitTypeDef led_config = {
            .Pin = GPIO_PIN_0,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
            .Alternate = 0
        };
          
           
        GPIO_TypeDef* led_port = GPIOB;
    
        auto temp = addComponent<TemperatureSensorInput>("Temp");
        auto output = addComponent<DigitalOutput>("digiout", led_port,&led_config);

        //addCoupling(digitalinput->out,atomique->in);
        addCoupling(temp->out, output->in);
    }
};

#endif  // SAMPLE_TOP_HPP
