/* This file is generated from chip_rename.def by genrename. */

/* This file is included only when chip_rename.h has been included. */
#ifdef TOPPERS_CHIP_RENAME_H
#undef TOPPERS_CHIP_RENAME_H

/*
 *  chip_config.c
 */
#undef chip_mprc_initialize
#undef chip_initialize
#undef chip_exit
#undef x_config_int
#undef ipi_ext_handler
#undef default_int_handler

/*
 *  chip_support.S
 */
#undef irq_handler
#undef target_exc_handler
#undef target_ret_exc
#undef target_ret_int

/*
 *  kernel_cfg.c
 */
#undef p_inh_table
#undef p_cfgint_table

/*
 *  mpcore.c
 */
#undef mpcore_dcache_enable
#undef mpcore_dcache_disable
#undef mpcore_icache_enable
#undef mpcore_icache_disable
#undef mpcore_cache_enable
#undef mpcore_cache_disable
#undef mmu_map_memory
#undef scu_init
#undef dic_stop
#undef dic_init
#undef dic_set_target
#undef dic_set_pending
#undef dic_clear_pending
#undef dic_enable_int
#undef dic_disable_int
#undef dic_cpu_stop
#undef dic_set_priority
#undef dic_config
#undef dic_probe_int
#undef dic_cpu_init
#undef scu_counter_init
#undef mpcore_smp_mode_enable
#undef mpcore_mmu_init


#ifdef TOPPERS_LABEL_ASM

/*
 *  chip_config.c
 */
#undef _chip_mprc_initialize
#undef _chip_initialize
#undef _chip_exit
#undef _x_config_int
#undef _ipi_ext_handler
#undef _default_int_handler

/*
 *  chip_support.S
 */
#undef _irq_handler
#undef _target_exc_handler
#undef _target_ret_exc
#undef _target_ret_int

/*
 *  kernel_cfg.c
 */
#undef _p_inh_table
#undef _p_cfgint_table

/*
 *  mpcore.c
 */
#undef _mpcore_dcache_enable
#undef _mpcore_dcache_disable
#undef _mpcore_icache_enable
#undef _mpcore_icache_disable
#undef _mpcore_cache_enable
#undef _mpcore_cache_disable
#undef _mmu_map_memory
#undef _scu_init
#undef _dic_stop
#undef _dic_init
#undef _dic_set_target
#undef _dic_set_pending
#undef _dic_clear_pending
#undef _dic_enable_int
#undef _dic_disable_int
#undef _dic_cpu_stop
#undef _dic_set_priority
#undef _dic_config
#undef _dic_probe_int
#undef _dic_cpu_init
#undef _scu_counter_init
#undef _mpcore_smp_mode_enable
#undef _mpcore_mmu_init


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/common/core_unrename.h"

#endif /* TOPPERS_CHIP_RENAME_H */
