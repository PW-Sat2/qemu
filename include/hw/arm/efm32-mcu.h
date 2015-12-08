#ifndef EFM32_MCU_H_
#define EFM32_MCU_H_

#include "hw/boards.h"
#include "hw/arm/cortexm-mcu.h"
#include "hw/arm/efm32-capabilities.h"

#define TYPE_EFM32_MCU "efm32-mcu"

/* Parent definitions. */
#define TYPE_EFM32_MCU_PARENT TYPE_CORTEXM_MCU

typedef CortexMClass EFM32MCUParentClass;
typedef CortexMState EFM32MCUParentState;

/* Class definitions. */
#define EFM32_MCU_GET_CLASS(obj) \
    OBJECT_GET_CLASS(EFM32MCUClass, (obj), TYPE_EFM32_MCU)
#define STM32_MCU_CLASS(klass) \
    OBJECT_CLASS_CHECK(EFM32MCUClass, (klass), TYPE_EFM32_MCU)

typedef struct EFM32MCUClass {
    /*< private >*/
    EFM32MCUParentClass parent_class;
    /*< public >*/

} EFM32MCUClass;

/* Instance definitions. */
#define EFM32_MCU_STATE(obj) \
    OBJECT_CHECK(EFM32MCUState, (obj), TYPE_EFM32_MCU)

typedef struct EFM32MCUState {
    /*< private >*/
    EFM32MCUParentState parent_obj;
    /*< public >*/

    /* Constructor parameters. */
    const EFM32Capabilities *param_capabilities;

    /* Specific STM32 capabilities; Cortex-M capabilities are separate. */
    const EFM32Capabilities *capabilities;

    Object *container;          
} EFM32MCUState;

// G_INLINE_FUNC DeviceState *efm32_mcu_get_rcc_dev(DeviceState *dev)
// {
//     return DEVICE((EFM32_MCU_STATE(dev)->rcc));
// }

#endif
