#ifndef SOLDERINGSTATION_DISPLAY_H
#define SOLDERINGSTATION_DISPLAY_H

namespace core {
namespace display {

void tick();

extern float backlight, contrast;

void setBacklight(float percentage);
void setContrast(float percentage);

};  // namespace display
};  // namespace core

#endif //SOLDERINGSTATION_DISPLAY_H
