#ifndef TESTS_PLANTMODEL_H_
#define TESTS_PLANTMODEL_H_

class TipModel {
 public:
    double powerScaling = 1.25,            // 20 -> 380 deg in 5 sec
           tipToJointConducance = 1.2,     // 20 -> 240 deg in 5 sec
           jointToBoardConductance = 0.3;  // 210 -> 60 deg in 5 sec

    const double tipThermalMass = 1.5,
                 jointThermalMass = 2;

    const double airLoss = 0.05;

    double _tipToJointConducance;

    double Ttip, Tjoint;
    const double Tboard = 20;
    double dt = 0.1;

    explicit TipModel(bool randomAir = false) : randomAir(randomAir) {
        std::srand(std::time(0));
        Ttip = Tboard;
        Tjoint = Tboard;
        soldering(false);
    }

    void soldering(bool solder) {
        if (solder) {
            _tipToJointConducance = tipToJointConducance;
        } else {
            _tipToJointConducance = 0;
        }
    }

    double airLossNow() {
        return airLoss*(Ttip - Tboard) + (randomAir ? (std::rand() % 10 - 5) : 0);
    }

    void tick(float pwr) {
        // pwr in 0..100

        double dTtipdt = powerScaling * pwr - _tipToJointConducance * (Ttip - Tjoint) - airLossNow();
        double dTjointdt = _tipToJointConducance*(Ttip - Tjoint) - jointToBoardConductance*(Tjoint - Tboard);

        Ttip += dTtipdt/tipThermalMass * dt;
        Tjoint += dTjointdt/jointThermalMass * dt;
    }

    bool randomAir;
};

#endif  // TESTS_PLANTMODEL_H_
