#ifndef SOLDERINGSTATION_CORE_SUPPORT_H
#define SOLDERINGSTATION_CORE_SUPPORT_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile bool tickTimeElapsed;

void MY_main();
void HAL_CmdCallback(char* buf);

#ifdef __cplusplus
}
#endif

#endif  // SOLDERINGSTATION_CORE_SUPPORT_H
