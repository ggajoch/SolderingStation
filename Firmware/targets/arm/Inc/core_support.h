#pragma once

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
