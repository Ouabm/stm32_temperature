#ifndef RT_DHTSENSORINTERFACE_HPP
#define RT_DHTSENSORINTERFACE_HPP

extern "C" {
#include "DHT_11/DHT.h" // Ton driver
}
MX_TIM1_Init();   
HAL_TIM_Base_Start(&htim1);

inline int read_temperature() {
    if (DHT11_Start()) {
        int RHI = DHT11_Read();
        int RHD = DHT11_Read();
        int TCI = DHT11_Read();
        int TCD = DHT11_Read();
        int SUM = DHT11_Read();

        if (RHI + RHD + TCI + TCD == SUM) {
            return TCI; // Température entière
        }
    }
    return -1000; // Code d'erreur
}

#endif
