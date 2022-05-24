#include "cmsis_os.h"
#include "BSP.h"

extern "C" void application(void);

class cl {
    int a;
    char b;
public:
    cl() {
        a = 1000;
        b = 'c';
    }
    ~cl() {
    }
};

void application(void)
{
    cl cll;
    //cl *pCl = new cl;

    //delete pCl;
    while (true) {
        osDelay(500);
        BSP_ledToggle();
    }
}