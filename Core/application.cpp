#include "cmsis_os.h"
#include "BSP.h"
#include "application.h"

void application(void)
{
    while (true) {
        osDelay(500);
        BSP_greenLedToggle();
    }
}