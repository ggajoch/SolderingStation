#ifndef CORE_COM_H_
#define CORE_COM_H_

namespace core {
namespace com {

int printf(const char* format, ...);
void rxCommandCallback(char* data);
void tick();

}  // namespace com
}  // namespace core

#endif  // CORE_COM_H_
