#include "hw/arm/efm32-mcu.h"
#include "hw/arm/cortexm-helper.h"

#if defined(CONFIG_VERBOSE)
#include "verbosity.h"
#endif

static void efm32_mcu_realize_callback(DeviceState *dev, Error **errp)
{
  qemu_log_function_name();

  if (!cm_device_parent_realize(dev, errp, TYPE_EFM32_MCU)) {
      return;
  }

  EFM32MCUState *state = EFM32_MCU_STATE(dev);
  const EFM32Capabilities *capabilities = state->param_capabilities;
  assert(capabilities != NULL);
  state->capabilities = capabilities;

  state->container = container_get(OBJECT(dev), "/efm32");

  {
    CortexMState *cm_state = CORTEXM_MCU_STATE(dev);
    int flash_size = cm_state->flash_size_kb * 1024;

    /* Allocate a new region for the alias */
    MemoryRegion *flash_alias_mem = g_malloc(sizeof(MemoryRegion));

    Object *mem_container = container_get(cm_state->container, "/memory");

    /* Initialise the new region */
    memory_region_init_alias(flash_alias_mem, mem_container, "mem-flash-alias", &cm_state->flash_mem, 0, flash_size);
    memory_region_set_readonly(flash_alias_mem, true);

    memory_region_add_subregion(get_system_memory(), 0x08000000, flash_alias_mem);
  }
}

static void efm32_mcu_reset_callback(DeviceState *dev)
{
  qemu_log_function_name();

  /* Call parent reset(). */
  cm_device_parent_reset(dev, TYPE_EFM32_MCU);
}

static void efm32_mcu_memory_regions_create_callback(DeviceState *dev)
{
  qemu_log_function_name();

  /* Create the parent (Cortex-M) memory regions */
  CortexMClass *parent_class = CORTEXM_MCU_CLASS(object_class_by_name(TYPE_CORTEXM_MCU));
  parent_class->memory_regions_create(dev);
}

#define DEFINE_PROP_EFM32CAPABILITIES_PTR(_n, _s, _f) \
    DEFINE_PROP(_n, _s, _f, qdev_prop_ptr, const EFM32Capabilities*)

static Property efm32_mcu_properties[] = {
        DEFINE_PROP_EFM32CAPABILITIES_PTR("efm32-capabilities", EFM32MCUState, param_capabilities),
        // DEFINE_PROP_UINT32("hse-freq-hz", EFM32MCUState, hse_freq_hz, 0),
        // DEFINE_PROP_UINT32("lse-freq-hz", EFM32MCUState, lse_freq_hz, 0),
    DEFINE_PROP_END_OF_LIST(), /**/
};

static void efm32_mcu_class_init_callback(ObjectClass *klass, void *data)
{
  DeviceClass *dc = DEVICE_CLASS(klass);
  dc->realize = efm32_mcu_realize_callback;
  dc->props = efm32_mcu_properties;
  dc->reset = efm32_mcu_reset_callback;

  CortexMClass *cm_class = CORTEXM_MCU_CLASS(klass);
  cm_class->memory_regions_create = efm32_mcu_memory_regions_create_callback;
}

static const TypeInfo efm32_mcu_type_info = {
  .abstract = true,
  .name = TYPE_EFM32_MCU,
  .parent = TYPE_EFM32_MCU_PARENT,
  .instance_size = sizeof(EFM32MCUState),
  .class_init = efm32_mcu_class_init_callback,
  .class_size = sizeof(EFM32MCUClass)
};

static void efm32_type_init(void)
{
  type_register_static(&efm32_mcu_type_info);
}

type_init(efm32_type_init);
