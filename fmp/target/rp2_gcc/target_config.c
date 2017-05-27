/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2007-2017 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  @(#) $Id: target_config.c 1218 2017-05-28 00:00:00Z azo $
 */

/*
 * チップ依存モジュール（Raspberry Pi用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "bcm283x.h"    /* for DDR_ADDR, SCCTRL_TIMEREN1SEL_TIMCLK */
#include "chip_serial.h" /* for vexpressa9_xxx */
#include "target_gpio.h"
#include <stdlib.h>

#ifdef TOPPERS_ENABLE_TRACE
#include <stdio.h>
static FILE *trace_log_file;
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  SIL_LOC_SPN()を取得したプロセッサの管理
 */
uint32_t TOPPERS_spn_var;

/*
 *  str_ker() の前でマスタプロセッサで行う初期化
 */
void
target_mprc_initialize(void)
{
        chip_mprc_initialize();

        /*
         * GPIOを初期化
         */
	target_gpio_initialize();

	/*
	 * syslogのためにtarget_fput_logを使用可にする
	 */
	sio_opn_por(1, 0);
}

/*
 *  exit()の最後に呼び出される関数．SkyEyeを終了させる．
 *  マスタプロセッサ以外が呼び出す．
 */
extern void _exit(int) NoReturn;

/*
 *  ターゲット依存の文字出力
 */
void
target_fput_log(char c)
{
        if (c == '\n') {
                bcm283x_putc('\r');
        }
        bcm283x_putc(c);
}

/*
 * MMU initialization (called from the chip code)
 *
 * We create a simpe 1-to-1 (physical address = virtual address) memory
 * mapping and enable caching for the DDR memory.
 *
 * Note: when not using SafeG the default world is Secure. When using
 * SafeG, Secure world uses secure accesses by default. If you want to
 * access a shared memory region with non-secure access, you will have
 * to configure it manually.
 */
void
target_mmu_init(void)
{
        MEMORY_ATTRIBUTE m_attribute;

        /*
         *  Basic initialization of the whole virtual memory space
         */

        /*
         *  Enable caching for the SDRAM
         *  Write-Back, Write-Allocate, Normal.
         */
        m_attribute.pa   = 0x00000000;
        m_attribute.va   = m_attribute.pa;
        m_attribute.size = 0x3F000000;
#if defined(TOPPERS_SAFEG_SECURE) || defined(TOPPERS_NOSAFEG)
        m_attribute.ns   = 1;     /* 0=Secure */
#endif
//        m_attribute.s    = 1;     /* 1=Shared */
        m_attribute.s    = 0;     /* 1=Shared */
        m_attribute.ap   = 3;     /* Full access */
        m_attribute.tex  = 0;     /* Outer and Inner */
        m_attribute.c    = 1;     /* Inner Write-Back */
        m_attribute.b    = 0;     /* Write Allocate */

        mmu_map_memory(&m_attribute);

        /*
         *  Enable caching for the Peripheral 0x3F000000-0x40FFFFFF
         *  Strongry-Ordered.
         */
        m_attribute.pa   = 0x3F000000;
        m_attribute.va   = m_attribute.pa;
        m_attribute.size = 0x02000000;
#if defined(TOPPERS_SAFEG_SECURE) || defined(TOPPERS_NOSAFEG)
        m_attribute.ns   = 1;     /* 0=Secure */
#endif
//        m_attribute.s    = 1;     /* 1=Shared */
        m_attribute.s    = 0;     /* 1=Shared */
        m_attribute.ap   = 3;     /* Full access */
        m_attribute.tex  = 0;     /* Outer and Inner */
        m_attribute.c    = 0;     /* Inner Write-Back */
        m_attribute.b    = 0;     /* Write Allocate */

        mmu_map_memory(&m_attribute);
}

#ifdef TOPPERS_SAFEG_SECURE
/*
 *  Fist swith to NT code(target_support.S)
 */
extern uint8_t first_switch_to_nt;
extern uint8_t first_switch_to_nt_end;

static void
copy_first_switch_to_nt(void){
	int cnt;
	volatile uint8_t *dst_addr;

	dst_addr = (uint8_t*)OCM_ADDR;
	for (cnt = 0; cnt < (&first_switch_to_nt_end - &first_switch_to_nt); cnt++) {
		*(dst_addr + cnt) = *(volatile uint8_t *)(&first_switch_to_nt + cnt);
	}

	__asm__ volatile("dsb");
	__asm__ volatile("isb");
}
#endif /* TOPPERS_SAFEG_SECURE */

#ifdef TOPPERS_SAFEG_SECURE
#include "syscalls_api.h"
#include "pl310.h"

/*
 * L2 cache initialize syscall
 */
uint32_t
safeg_l2cache_init(const uint32_t core_id,
				   const uint32_t ns,
				   const uint32_t aux_val,
				   const uint32_t aux_mask,
				   const uint32_t c)
{
	pl310_init(aux_val, aux_mask);
	return SAFEG_OK(SAFEG_SYSCALL_RET__DONT_SWITCH);
}

struct safeg_syscall l2init_call = {
	.is_t_callable  = 1,
	.is_nt_callable = 1,
	.name = "l2init",
	.function = safeg_l2cache_init,
};

/*
 * Regist L2 cache debug register warite syscall
 */
uint32_t
l2cache_debug_set(const uint32_t core_id,
				  const uint32_t ns,
				  const uint32_t a,
				  const uint32_t b,
				  const uint32_t c)
{
	pl310_debug_set(a);
	return SAFEG_OK(SAFEG_SYSCALL_RET__DONT_SWITCH);
}

struct safeg_syscall l2debug_set_call = {
	.is_t_callable  = 1,
	.is_nt_callable = 1,
	.name = "l2debug",
	.function = l2cache_debug_set,
};

/*
 * Boot second core
 */
uint32_t
safeg_boot_second_core(const uint32_t core_id,
				   const uint32_t ns,
				   const uint32_t address,
				   const uint32_t b,
				   const uint32_t c)
{
	boot_second_core(address);
	return SAFEG_OK(SAFEG_SYSCALL_RET__DONT_SWITCH);
}

struct safeg_syscall boot_second_core_call = {
	.is_t_callable  = 1,
	.is_nt_callable = 1,
	.name = "bsecond",
	.function = safeg_boot_second_core,
};

#endif /* TOPPERS_SAFEG_SECURE */

/*
 *  Target-dependent initialization
 */
extern SIOPCB *bcm283x_uart_opn_por(SIOPCB *p_siopcb, intptr_t exinf);

#if !defined(TOPPERS_SAFEG_SECURE)
volatile uint32_t target_initialize_end[TNUM_PRCID];
#endif /* !TOPPERS_SAFEG_SECURE */

void
target_initialize(void)
{
	/*
	 *  チップ依存の初期化
	 */
	chip_initialize();

	/*
	 *  Initialize the UART. If we are using a
	 *  global logger (shared by all cores) only
	 *  the master processor (mprc) initializes it.
	 */
#ifdef G_SYSLOG
	if (x_sense_mprc()) {
		sio_opn_por(1, 0);
	}
#else /* !G_SYSLOG */
	sio_opn_por(1, 0);
#endif /* G_SYSLOG */


#ifdef TOPPERS_SAFEG_SECURE
	/* Set NULL(0x00) for Linux second core boot */
	sil_wrw_mem((uint32_t*)BOOT_MEM_ADDR, 0x00);
	copy_first_switch_to_nt();
#endif /* TOPPERS_SAFEG_SECURE */

#ifdef TOPPERS_SAFEG_SECURE
	if (x_sense_mprc()) {
		/* Regist L2 cache initialize syscall */
		uint32_t id;
		safeg_syscall_regdyn(&l2init_call, &id);

		/* Regist L2 cache debug register write syscall */
		safeg_syscall_regdyn(&l2debug_set_call, &id);

		/* Regist boot second core syscall */
		safeg_syscall_regdyn(&boot_second_core_call, &id);
	}
#endif /*  TOPPERS_SAFEG_SECURE */

#ifdef TOPPERS_NOSAFEG
	/* Enable L2 cache by myself in case of without safeg or with safeg Non-Secure */
	if (x_sense_mprc()) {
		uint32_t i;
		uint32_t sum = 0;
		while(1) {
			for(i = 1; i < TNUM_PRCID; i++) {
				sum += target_initialize_end[i];
			}
			if (sum == (TNUM_PRCID - 1)) {
				break;
			}
		}
		pl310_init(0, ~0x0U);
	}
	target_initialize_end[x_prc_index()] = 1;
#endif /* TOPPERS_NOSAFEG */
}

/*
 *  Target-dependent exit processing
 */
void
target_exit(void)
{
	/*
	 *  チップ依存の終了処理
	 */
	chip_exit();

	/*
	 *  Endless loop
	 */
	while(1);
}
