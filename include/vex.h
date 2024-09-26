#include "v5.h"
#include "v5_vcs.h"

#define waitUntil(condition)                                                   \
    do {                                                                       \
        wait(5, msec);                                                         \
    } while (!(condition))

#define repeat(iterations)                                                     \
    for (int iterator = 0; iterator < iterations; iterator++)

// ----------define robot color---------
#define RED_ALLIANCE
// #define BLUE_ALLIANCE

// ----------define competition---------
#define COMPETITION
// #define SKILL
#ifndef COMPETITION
// #define DEBUG
#endif