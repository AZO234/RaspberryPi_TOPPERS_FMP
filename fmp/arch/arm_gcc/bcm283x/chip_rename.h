/* This file is generated from chip_rename.def by genrename. */

#ifndef TOPPERS_CHIP_RENAME_H
#define TOPPERS_CHIP_RENAME_H

/*
 *  chip_config.c
 */
#define chip_mprc_initialize		_kernel_chip_mprc_initialize
#define chip_initialize				_kernel_chip_initialize
#define chip_exit					_kernel_chip_exit
#define default_int_handler			_kernel_default_int_handler

/*
 *  kernel_cfg.c
 */
#define p_inh_table					_kernel_p_inh_table
#define p_private_cfgint_table		_kernel_p_private_cfgint_table
#define global_cfgint_table			_kernel_global_cfgint_table

/*
 *  mpcore.c
 */
#define mmu_map_memory				_kernel_mmu_map_memory
#define scu_init					_kernel_scu_init
#define scu_counter_init			_kernel_scu_counter_init
#define smp_mode_enable				_kernel_smp_mode_enable
#define mmu_init					_kernel_mmu_init


#ifdef TOPPERS_LABEL_ASM

/*
 *  chip_config.c
 */
#define _chip_mprc_initialize		__kernel_chip_mprc_initialize
#define _chip_initialize			__kernel_chip_initialize
#define _chip_exit					__kernel_chip_exit
#define _default_int_handler		__kernel_default_int_handler

/*
 *  kernel_cfg.c
 */
#define _p_inh_table				__kernel_p_inh_table
#define _p_private_cfgint_table		__kernel_p_private_cfgint_table
#define _global_cfgint_table		__kernel_global_cfgint_table

/*
 *  mpcore.c
 */
#define _mmu_map_memory				__kernel_mmu_map_memory
#define _scu_init					__kernel_scu_init
#define _scu_counter_init			__kernel_scu_counter_init
#define _smp_mode_enable			__kernel_smp_mode_enable
#define _mmu_init					__kernel_mmu_init


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/common/core_rename.h"

#endif /* TOPPERS_CHIP_RENAME_H */
