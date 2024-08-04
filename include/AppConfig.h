
#ifndef INCLUDE_APPCONFIG_H_
#define INCLUDE_APPCONFIG_H_

#include <FreeRTOS.h>

/* memory sectiona address from linker script */
extern BaseType_t __device_settings_start;
extern BaseType_t __device_settings_end;

BaseType_t * const appconfigDEVICE_SETTINGS_BASE_ADDRESS = &__device_settings_start;

#endif /* INCLUDE_APPCONFIG_H_ */
