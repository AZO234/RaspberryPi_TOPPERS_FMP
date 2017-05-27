/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2013 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.h 1024 2017-05-28 00:00:00Z azo $
 */

/*
 *  This file contains some configuration-dependent information
 *  and some prototype functions.
 */
#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  Enable trace (use the ENABLE_TRACE flag at the app's Makefile)
 *
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  Spinlock implementation to use
 *      - NATIVE_SPN: no contention with other syscalls
 *      - EMULATE_SPN: contention with other syscalls
 */
#define TTYPE_SPN NATIVE_SPN
//#define TTYPE_SPN EMULATE_SPN

/*
 *  Maximum number of unsuccesful spins (Native Spinlock)
 */
#define TMAX_NATIVE_SPN   256

/*
 *  Default size of non-task stack for storing context
 */
#define DEFAULT_PRC1_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC2_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC3_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC4_ISTKSZ      (1024 * 8)   /* 8KB */

/*
 *  Include chip configuration (BCM283X)
 */
#include "arm_gcc/bcm283x/chip_config.h"

#ifndef TOPPERS_MACRO_ONLY

/*
 * Memory map initialization (called from the chip-dependent code)
 */
extern void target_mmu_init(void);

/*
 *  Target-dependent initialization
 */
extern void target_initialize(void);

/*
 *  Function to execute when the system exits
 */
extern void target_exit(void) NoReturn;

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_TARGET_CONFIG_H */
