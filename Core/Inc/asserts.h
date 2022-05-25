#pragma once

extern void BSP_disableInterrupt(void);

#ifdef DEBUG
#   define ASSERT(x, ...)    do {                                                      \
                                 if ( !( x ) ) { BSP_disableInterrupt(); for (;;) {} } \
                             } while (0)
#else
#   define ASSERT(x, ...)    ((void)0)
#endif