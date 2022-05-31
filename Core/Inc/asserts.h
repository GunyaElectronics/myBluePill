#pragma once

extern void BSP_disableInterrupt(void);

#define THIS_IS_UNDEFINED_STATE     (0)
#ifdef DEBUG
#   define ASSERT(x, ...)    do {                                                      \
                                 if ( !( x ) ) { BSP_disableInterrupt(); for (;;) {} } \
                             } while (0)
#else
#   define ASSERT(x, ...)    ((void)0)
#endif