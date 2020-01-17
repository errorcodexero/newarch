#include <hal/Main.h>
#include <stdlib.h>

void HAL_ExitMain()
{
    exit(0);
}

HAL_Bool HAL_HasMain()
{
    return 1;
}

void HAL_RunMain()
{
}