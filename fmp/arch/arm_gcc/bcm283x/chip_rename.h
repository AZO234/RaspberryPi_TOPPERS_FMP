/* This file is generated from chip_rename.def by genrename. */

#ifndef TOPPERS_CHIP_RENAME_H
#define TOPPERS_CHIP_RENAME_H

/*
 *  chip_config.c
 */
#define chip_mprc_initialize		_kernel_chip_mprc_initialize
#define chip_initialize				_kernel_chip_initialize
#define chip_exit					_kernel_chip_exit
#define x_config_int				_kernel_x_config_int
#define ipi_ext_handler				_kernel_ipi_ext_handler
#define default_int_handler			_kernel_default_int_handler

/*
 *  chip_support.S
 */
#define irq_handler					_kernel_irq_handler
#define target_exc_handler			_kernel_target_exc_handler
#define target_ret_exc				_kernel_target_ret_exc
#define target_ret_int				_kernel_target_ret_int

/*
 *  kernel_cfg.c
 */
#define p_inh_table					_kernel_p_inh_table
#define p_cfgint_table				_kernel_p_cfgint_table

/*
 *  mpcore.c
 */
#define mpcore_dcache_enable		_kernel_mpcore_dcache_enable
#define mpcore_dcache_disable		_kernel_mpcore_dcache_disable
#define mpcore_icache_enable		_kernel_mpcore_icache_enable
#define mpcore_icache_disable		_kernel_mpcore_icache_disable
#define mpcore_cache_enable			_kernel_mpcore_cache_enable
#define mpcore_cache_disable		_kernel_mpcore_cache_disable
#define mmu_map_memory				_kernel_mmu_map_memory
#define scu_init					_kernel_scu_init
#define dic_stop					_kernel_dic_stop
#define dic_init					_kernel_dic_init
#define dic_set_target				_kernel_dic_set_target
#define dic_set_pending				_kernel_dic_set_pending
#define dic_clear_pending			_kernel_dic_clear_pending
#define dic_enable_int				_kernel_dic_enable_int
#define dic_disable_int				_kernel_dic_disable_int
#define dic_cpu_stop				_kernel_dic_cpu_stop
#define dic_set_priority			_kernel_dic_set_priority
#define dic_config					_kernel_dic_config
#define dic_probe_int				_kernel_dic_probe_int
#define dic_cpu_init				_kernel_dic_cpu_init
#define scu_counter_init			_kernel_scu_counter_init
#define mpcore_smp_mode_enable		_kernel_mpcore_smp_mode_enable
#define mpcore_mmu_init				_kernel_mpcore_mmu_init


#ifdef TOPPERS_LABEL_ASM

/*
 *  chip_config.c
 */
#define _chip_mprc_initialize		__kernel_chip_mprc_initialize
#define _chip_initialize			__kernel_chip_initialize
#define _chip_exit					__kernel_chip_exit
#define _x_config_int				__kernel_x_config_int
#define _ipi_ext_handler			__kernel_ipi_ext_handler
#define _default_int_handler		__kernel_default_int_handler

/*
 *  chip_support.S
 */
#define _irq_handler				__kernel_irq_handler
#define _target_exc_handler			__kernel_target_exc_handler
#define _target_ret_exc				__kernel_target_ret_exc
#define _target_ret_int				__kernel_target_ret_int

/*
 *  kernel_cfg.c
 */
#define _p_inh_table				__kernel_p_inh_table
#define _p_cfgint_table				__kernel_p_cfgint_table

/*
 *  mpcore.c
 */
#define _mpcore_dcache_enable		__kernel_mpcore_dcache_enable
#define _mpcore_dcache_disable		__kernel_mpcore_dcache_disable
#define _mpcore_icache_enable		__kernel_mpcore_icache_enable
#define _mpcore_icache_disable		__kernel_mpcore_icache_disable
#define _mpcore_cache_enable		__kernel_mpcore_cache_enable
#define _mpcore_cache_disable		__kernel_mpcore_cache_disable
#define _mmu_map_memory				__kernel_mmu_map_memory
#define _scu_init					__kernel_scu_init
#define _dic_stop					__kernel_dic_stop
#define _dic_init					__kernel_dic_init
#define _dic_set_target				__kernel_dic_set_target
#define _dic_set_pending			__kernel_dic_set_pending
#define _dic_clear_pending			__kernel_dic_clear_pending
#define _dic_enable_int				__kernel_dic_enable_int
#define _dic_disable_int			__kernel_dic_disable_int
#define _dic_cpu_stop				__kernel_dic_cpu_stop
#define _dic_set_priority			__kernel_dic_set_priority
#define _dic_config					__kernel_dic_config
#define _dic_probe_int				__kernel_dic_probe_int
#define _dic_cpu_init				__kernel_dic_cpu_init
#define _scu_counter_init			__kernel_scu_counter_init
#define _mpcore_smp_mode_enable		__kernel_mpcore_smp_mode_enable
#define _mpcore_mmu_init			__kernel_mpcore_mmu_init


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/common/core_rename.h"

#endif /* TOPPERS_CHIP_RENAME_H */
