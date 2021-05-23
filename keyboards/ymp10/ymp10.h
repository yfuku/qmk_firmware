#pragma once

#include "quantum.h"
#define kNN KC_NO

#define LAYOUT( \
              k02, k03, \
    k10, k11, k12, k13, \
    k20, k21, k22, k23   \
) { \
    {   0,   0, k02, k03 }, \
    { k10, k11, k12, k13 }, \
    { k20, k21, k22, k23 }, \
}
