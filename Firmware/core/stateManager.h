#ifndef CORE_SLEEPMANAGER_H_
#define CORE_SLEEPMANAGER_H_

namespace core {
namespace stateManager {

extern bool sleep, in_stand, configuration_correct;

void tick();

}  // namespace stateManager
}  // namespace core

#endif  // CORE_SLEEPMANAGER_H_
