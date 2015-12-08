#include "hw/arm/efm32-mcus.h"
#include "hw/arm/cortexm-mcu.h"
#include "exec/address-spaces.h"
#include "qemu/error-report.h"
#include "sysemu/sysemu.h"
#include "hw/arm/cortexm-helper.h"

#if defined(CONFIG_VERBOSE)
#include "verbosity.h"
#endif

static const CortexMCoreCapabilities efm32gg_core = {
  .cpu_model = "cortex-m3",
  .has_mpu = true,
  .has_etm = false,
  .has_itm = true,
  .num_irq = 44,
  .nvic_bits = 4
};

static const EFM32Capabilities efm32gg990 = {
  .family = EFM32_FAMILY_GG
};

static const EFM32PartInfo efm32_mcus[] = {
  {
    .name = TYPE_EFM32GG990F1024,
    .cortexm = {
      .flash_size_kb = 1024,
      .sram_size_kb = 128,
      .core = &efm32gg_core
    },
    .efm32 = &efm32gg990
  },
  {
    .name = 0
  }
};

static void efm32_mcus_realize_callback(DeviceState *dev, Error **errp)
{
    qemu_log_function_name();

    EFM32DeviceClass *st_class = EFM32_DEVICE_GET_CLASS(dev);
    EFM32PartInfo *part_info = st_class->part_info;

    /*
     * Set additional constructor parameters, that were passed via
     * the .class_data and copied to custom class member.
     */
    qdev_prop_set_ptr(dev, "cortexm-capabilities", (void *) &part_info->cortexm);
    qdev_prop_set_ptr(dev, "efm32-capabilities", (void *) part_info->efm32);

    /*
     * Call parent realize().
     * We do not know the current typename, since it was generated
     * with a table, so we use the parent typename.
     */
    if (!cm_device_by_name_realize(dev, errp, TYPE_EFM32_DEVICE_PARENT)) {
        return;
    }
}

static void efm32_mcus_reset_callback(DeviceState *dev)
{
    qemu_log_function_name();

    /* Call parent reset(). */
    cm_device_by_name_reset(dev, TYPE_EFM32_MCU);
}

static Property efm32_mcus_properties[] = {
    DEFINE_PROP_END_OF_LIST(), /**/
};

static void efm32_mcus_class_init_callback(ObjectClass *klass, void *data)
{
  DeviceClass *dc = DEVICE_CLASS(klass);
  dc->realize = efm32_mcus_realize_callback;
  dc->reset = efm32_mcus_reset_callback;
  dc->props = efm32_mcus_properties;

  EFM32DeviceClass *st_class = (EFM32DeviceClass *) (klass);
  /*
   * Copy the 'data' param, pointing to a ParamInfo, to the class
   * structure, to be retrieved by the constructor.
   */
  st_class->part_info = data;
}

static void efm32_mcus_types_init(void)
{
  int i;
  for (i = 0; efm32_mcus[i].name; ++i) {
    TypeInfo ti = {
        .name = efm32_mcus[i].name,
        .parent = TYPE_EFM32_DEVICE_PARENT,
        .instance_size = sizeof(EFM32DeviceState),
        .class_init = efm32_mcus_class_init_callback,
        .class_size = sizeof(EFM32DeviceClass),
        .class_data = (void *) &efm32_mcus[i] };
    type_register(&ti);
  }
}

type_init(efm32_mcus_types_init);
