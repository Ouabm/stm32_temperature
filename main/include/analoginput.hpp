#ifndef RT_ANALOGINPUT_HPP
#define RT_ANALOGINPUT_HPP

#include "cadmium/modeling/devs/atomic.hpp"
#include "stm32h7xx_hal_adc.h"

using namespace std;

namespace cadmium {

    struct AnalogInputState {
        double output;
        double last;
        double sigma;

        AnalogInputState(): output(0), last(0), sigma(0) {}
    };

    std::ostream& operator<<(std::ostream &out, const AnalogInputState& state) {
        out << "Valeur analogique: " << state.output;
        return out;
    }

    class AnalogInput : public Atomic<AnalogInputState> {
    public:
        Port<double> out;

        // Pointeur vers le handle ADC
        ADC_HandleTypeDef* analogPin;

        // Période d’échantillonnage (en secondes)
        double pollingRate;

        // Constructeur
        AnalogInput(const std::string& id, ADC_HandleTypeDef* pin) 
            : Atomic<AnalogInputState>(id, AnalogInputState()), analogPin(pin), pollingRate(0.01) {

            out = addOutPort<double>("out");

            // Première lecture
            HAL_ADC_Start(analogPin);
            HAL_ADC_PollForConversion(analogPin, 10); // timeout 10 ms

            if (HAL_IS_BIT_SET(HAL_ADC_GetState(analogPin), HAL_ADC_STATE_REG_EOC)) {
                state.output = (double) HAL_ADC_GetValue(analogPin);
                state.last = state.output;
            }

            state.sigma = pollingRate;
        }

        // Transition interne : lire une nouvelle valeur ADC
        void internalTransition(AnalogInputState& state) const override {
            state.last = state.output;

            HAL_ADC_Start(analogPin);
            HAL_ADC_PollForConversion(analogPin, 10); // timeout 10 ms

            if (HAL_IS_BIT_SET(HAL_ADC_GetState(analogPin), HAL_ADC_STATE_REG_EOC)) {
                state.output = (double) HAL_ADC_GetValue(analogPin);
            }

            state.sigma = pollingRate;
        }

        // Transition externe (non utilisée ici)
        void externalTransition(AnalogInputState& state, double e) const override {
            // Pas d’entrée, aucune action à effectuer ici
        }

        // Fonction de sortie
        void output(const AnalogInputState& state) const override {
            if (state.output != state.last) {
                out->addMessage(state.output);
            }
        }

        // Avance dans le temps
        [[nodiscard]] double timeAdvance(const AnalogInputState& state) const override {
            return state.sigma;
        }
    };
}

#endif // RT_ANALOGINPUT_HPP
