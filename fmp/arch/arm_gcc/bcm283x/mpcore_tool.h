/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2006-2012 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: mpcore_tool.h 1034 2013-10-05 06:14:05Z ertl-honda $
 */

/*
 *  MPCORE のハードウェア資源のツール依存部の定義
 */

#ifndef TOPPERS_MPCORE_TOOL_H
#define TOPPERS_MPCORE_TOOL_H

#ifndef TOPPERS_MACRO_ONLY

/*
 *  コプロへのアクセスマクロ
 */
#define CP15_CONTROL_READ(x)     Asm("mrc p15, 0, %0, c1, c0, 0":"=r"(x))
#define CP15_CONTROL_WRITE(x)    Asm("mcr p15, 0, %0, c1, c0, 0"::"r"(x))

#define CP15_AUXILIARY_READ(x)   Asm("mrc p15, 0, %0, c1, c0, 1":"=r"(x))
#define CP15_AUXILIARY_WRITE(x)  Asm("mcr p15, 0, %0, c1, c0, 1"::"r"(x))

#define CP15_ICACHE_INVALIDATE()           Asm("mcr p15, 0, %0, c7, c5, 0"::"r"(0))
#define CP15_BRANCHP_INVALIDATE()          Asm("mcr p15, 0, %0, c7, c5, 6"::"r"(0))

#if __TARGET_ARCH_ARM == 6
#define CP15_DCACHE_INVALIDATE()           Asm("mcr p15, 0, %0, c7, c6, 0"::"r"(0))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE() Asm("mcr p15, 0, %0, c7, c14, 0"::"r"(0))
#elif __TARGET_ARCH_ARM == 7
#define CP15_BPI_INVALIDATEALL(x)           Asm("mcr p15, 0, %0, c7, c5, 6"::"r"(x))
#define CP15_DCACHE_INVALIDATE(x)           Asm("mcr p15, 0, %0, c7, c6, 2"::"r"(x))
#define CP15_DCACHE_CLEAN(x)                Asm("mcr p15, 0, %0, c7, c10,2"::"r"(x))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE(x) Asm("mcr p15, 0, %0, c7, c14,2"::"r"(x))
#endif /* __TARGET_ARCH_ARM == 7 */

#if __TARGET_ARCH_ARM == 6
#define CP15_PBUFFER_FLUSH()               Asm("mcr p15, 0, %0, c7, c5, 4"::"r"(0):"memory")
#define CP15_DATA_SYNC_BARRIER()           Asm("mcr p15, 0, %0, c7, c10, 4"::"r"(0):"memory")
#define CP15_DATA_MEMORY_BARRIER()         Asm("mcr p15, 0, %0, c7, c10, 5"::"r" (0):"memory")
#elif __TARGET_ARCH_ARM == 7
#define CP15_PBUFFER_FLUSH()               Asm("isb":::"memory")
#define CP15_DATA_SYNC_BARRIER()           Asm("dsb":::"memory")
#define CP15_DATA_MEMORY_BARRIER()         Asm("dmb":::"memory")
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_INVALIDATE_UNIFIED_TLB() Asm("mcr p15, 0, %0, c8, c7,  0"::"r"(0))

#define CP15_TTBCR_WRITE(x) Asm("mcr p15, 0, %0, c2, c0, 2"::"r"(x))
#define CP15_TTB0_READ(x)   Asm("mrc p15, 0, %0, c2, c0, 0":"=r"(x))
#define CP15_TTB0_WRITE(x)  Asm("mcr p15, 0, %0, c2, c0, 0"::"r"(x))

#define CP15_DOMAINS_WRITE(x) Asm("mcr p15, 0, %0, c3, c0, 0":: "r"(x))

#define CP15_CPUID_READ(x)     Asm("mrc p15, 0, %0, c0, c0, 5":"=r"(x))

#if __TARGET_ARCH_ARM == 7
#define CP15_CACHE_SIZE_SELECTION_WRITE(x) Asm("mcr p15, 2, %0, c0, c0, 0"::"r"(x))
#define CP15_CACHE_SIZE_SELECTION_READ(x)  Asm("mrc p15, 2, %0, c0, c0, 0":"=r"(x))
#define CP15_NSACR_WRITE(x) Asm("mcr p15, 0, %0, c1, c1, 2"::"r"(x))
#define CP15_NSACR_READ(x)  Asm("mrc p15, 0, %0, c1, c1, 2":"=r"(x))
#define CP15_CSSELR_WRITE(x)   Asm("mcr p15, 2, %0, c0, c0, 0"::"r"(x))
#define CP15_CCSIDR_READ(x)    Asm("mrc p15, 1, %0, c0, c0, 0":"=r"(x))
#define CP15_CLIDR_READ(x)     Asm("mrc p15, 1, %0, c0, c0, 1":"=r"(x))
#endif /* __TARGET_ARCH_ARM == 7 */

#define CPU15_PONLY_THREAD_ID_READ(x)  Asm("mrc p15, 0, %0, c13, c0, 4":"=r"(x))
#define CPU15_PONLY_THREAD_ID_WRITE(x) Asm("mcr p15, 0, %0, c13, c0, 4":: "r"(x))

#define CP15_SET_VBAR(x)    Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define CP15_TLB_INVALIDATE_ALL()  Asm("mcr p15, 0, %0, c8, c7, 0" ::"r"(0))

#define CNTFRQ_READ(x)     Asm("mrc p15, 0, %0, c14, c0, 0":"=r"(x))
#define CNTPCT_READ(x,y)   Asm("mrrc p15, 0, %0, %1, c14":"=r"(x), "=r"(y))

#define CNTP_TVAL_WRITE(x)  Asm("mcr p15, 0, %0, c14, c2, 0"::"r"(x))

#define CNTP_CTL_READ(x)   Asm("mrc p15, 0, %0, c14, c2, 1":"=r"(x))
#define CNTP_CTL_WRITE(x)  Asm("mcr p15, 0, %0, c14, c2, 1"::"r"(x))

#define CNTP_CVAL_READ(x,y)  Asm("mrrc p15, 2, %0, %1, c14":"=r"(x), "=r"(y))
#define CNTP_CVAL_WRITE(x,y) Asm("mcrr p15, 2, %0, %1, c14"::"r"(x), "r"(y))
#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_MPCORE_TOOL_H */
