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
#undef default_int_handler

/*
 *  kernel_cfg.c
 */
#undef p_inh_table
#undef p_private_cfgint_table
#undef global_cfgint_table

/*
 *  mpcore.c
 */
#undef mmu_map_memory
#undef scu_init
#undef scu_counter_init
#undef smp_mode_enable
#undef mmu_init


#ifdef TOPPERS_LABEL_ASM

/*
 *  chip_config.c
 */
#undef _chip_mprc_initialize
#undef _chip_initialize
#undef _chip_exit
#undef _default_int_handler

/*
 *  kernel_cfg.c
 */
#undef _p_inh_table
#undef _p_private_cfgint_table
#undef _global_cfgint_table

/*
 *  mpcore.c
 */
#undef _mmu_map_memory
#undef _scu_init
#undef _scu_counter_init
#undef _smp_mode_enable
#undef _mmu_init


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/common/core_unrename.h"

#endif /* TOPPERS_CHIP_RENAME_H */
