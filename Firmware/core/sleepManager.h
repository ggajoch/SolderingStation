#ifndef CORE_SLEEPMANAGER_H_
#define CORE_SLEEPMANAGER_H_

namespace core {
namespace sleepManager {

extern bool sleepState, standState, configState;

void tick();
void encoderCallback();
void configStateSet(bool now);

}  // namespace sleepManager
}  // namespace core

#endif  // CORE_SLEEPMANAGER_H_
