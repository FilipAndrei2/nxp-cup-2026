#pragma once
/* Stub pwm.h for host-side unit testing.
 * Provides only the type aliases needed by Params.hpp. */
#include "PlatformTypes.h"

typedef uint8  Pwm_ChannelType;
typedef uint16 Pwm_PeriodType;

/* I2c type needed by Params.hpp (brought in transitively in the real BSW) */
typedef uint16 I2c_AddressType;
