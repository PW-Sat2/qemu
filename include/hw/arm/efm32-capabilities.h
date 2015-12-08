#ifndef EFM32_CAPABILITIES_H_
#define EFM32_CAPABILITIES_H_

#include <stdint.h>

#define TYPE_EFM32_PREFIX     "efm32:"
#define TYPE_EFM32GG_PREFIX   "efm32gg:"

typedef enum {
  EFM32_FAMILY_UNKNOWN,
  EFM32_FAMILY_GG
} efm32_family_t;

typedef struct {

  efm32_family_t  family;

  uint32_t hsi_freq_hz;
  uint32_t lsi_freq_hz;

} EFM32Capabilities;

#endif
