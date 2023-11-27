#pragma once

#include_next <mcuconf.h>

// probably unnecessary ?
#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE

