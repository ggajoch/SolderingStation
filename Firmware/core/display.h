#ifndef CORE_DISPLAY_H_
#define CORE_DISPLAY_H_

namespace core {
namespace display {

void tick();

extern float backlight, contrast;

void setBacklight(float percentage);
void setContrast(float percentage);

}  // namespace display
}  // namespace core

#endif  // CORE_DISPLAY_H_
