#ifndef ATOMIC_MODEL_HPP
#define ATOMIC_MODEL_HPP
#include "cadmium/modeling/devs/atomic.hpp"


using namespace cadmium;

struct atomic_modelState {

    double sigma;
    double rotate;
    bool fastToggle;
     bool buttonPressed;  // nouveau
    atomic_modelState() : sigma(0.0), rotate(false),fastToggle(true),buttonPressed(false) {}
};

std::ostream& operator<<(std::ostream& out, const atomic_modelState& state) {
   out << "Status:, " << state.rotate << ", sigma: " << state.sigma;
    return out;
}

class atomic_model : public Atomic<atomic_modelState> {
public:
    Port<double>out ;
    Port<bool> in;
    double slowToggleTime ;
    double fastToggleTime;
    // Constructeur : initialiser la LED sur la broche LED1
    atomic_model(const std::string& id) :
        Atomic<atomic_modelState>(id, atomic_modelState())
    {
        out = addOutPort<double>("out");
        in= addInPort<bool>("in");
        slowToggleTime = 10;
        fastToggleTime = 1;
        state.sigma=fastToggleTime;

            // Set a value for sigma (so it is not 0), this determines how often the
       
    }

    void internalTransition(atomic_modelState& state) const override {
        state.rotate=!state.rotate;
        state.sigma = 5.0;
    }

    void externalTransition(atomic_modelState& state, double e) const override {

    }

    void output(const atomic_modelState& state) const override {
        out->addMessage(state.rotate ? 1.0 : 0.0);// Pas besoin d’output explicite ici, car l’action se fait dans internalTransition
         
    }

    [[nodiscard]] double timeAdvance(const atomic_modelState& state) const override {
        return state.sigma;
    }
};
#endif
