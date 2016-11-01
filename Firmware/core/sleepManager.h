#ifndef SOLDERINGSTATION_SLEEPMANAGER_H
#define SOLDERINGSTATION_SLEEPMANAGER_H

namespace core {
namespace sleepManager {

extern bool sleepState, standState;

void tick();
void encoderCallback();

};  // namespace sleepManager
};  // namespace core


#endif  // SOLDERINGSTATION_SLEEPMANAGER_H
