#ifndef EFM32_MCUS_H_
#define EFM32_MCUS_H_

#include "hw/arm/efm32-mcu.h"

#define TYPE_EFM32GG990F1024 "EFM32GG990F1024"
#define TYPE_EFM32GG990F1024_BIG "EFM32GG990F1024_BIG"

/* Parent definitions. */
#define TYPE_EFM32_DEVICE_PARENT TYPE_EFM32_MCU
typedef EFM32MCUClass EFM32DeviceParentClass;
typedef EFM32MCUState EFM32DeviceParentState;

/* Class definitions. */
/* Warning, this cast does not check the type! */
#define EFM32_DEVICE_GET_CLASS(obj) \
    ((EFM32DeviceClass *)object_get_class(OBJECT(obj)))

/**
 * Structure to define the specifics of each MCU. Capabilities are
 * split between core & stm32; they care processed by parent class
 * constructors.
 */
typedef struct {

    const char *name; /* CMSIS device name */

    const CortexMCapabilities cortexm;
    const EFM32Capabilities *efm32;

} EFM32PartInfo;

typedef struct {
    /*< private >*/
    EFM32DeviceParentClass parent_class;
    /*< public >*/

    EFM32PartInfo *part_info;
} EFM32DeviceClass;

/* Instance definitions. */
typedef struct {
    /*< private >*/
    EFM32DeviceParentState parent_class;
    /*< public >*/

} EFM32DeviceState;

#endif
