/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2006-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  @(#) $Id: chip_kernel.h 955 2016-10-17 00:00:00Z azo $
 */

/*
 *  kernel.hのチップ依存部（BCM283X用）
 *
 *  このインクルードファイルは，kernel.hでインクルードされる．他のファ
 *  イルから直接インクルードすることはない．このファイルをインクルード
 *  する前に，t_stddef.hがインクルードされるので，それらに依存してもよ
 *  い．
 */

#ifndef TOPPERS_CHIP_KERNEL_H
#define TOPPERS_CHIP_KERNEL_H

#include "bcm283x.h"

/*
 *  ARMで共通な定義
 */
#include "arm_gcc/common/core_kernel.h"

/*
 *  Method for managing the system time
 *
 *  Note: the SP804 timers only use 1 interrupt so we have to use the
 *  same timer for all cores. The Cortex-A9 private timers and watchdogs
 *  have 1 interrupt per core so we can use them as local timers.
 */
#define TOPPERS_SYSTIM_GLOBAL /* use a global timer for all CPUs */

/*
 *  Master processor
 */
#define TOPPERS_MASTER_PRCID  1

/*
 *  Processor ID that manages the system time when using a global timer
 */
#define TOPPERS_SYSTIM_PRCID  TOPPERS_MASTER_PRCID

/*
 * Default number of processors (this value can be overwritten in the
 * application's Makefile with the desired number of processors)
 */
#ifndef TNUM_PRCID
#define TNUM_PRCID  4
#endif /* TNUM_PRCID */

/*
 *  Definition of the CLASS IDs which are used to assign tasks to processors
 *  statically at initialization.
 */
#define TCL_1          1 /* Assign tasks in TCL_1 to processor 1 */
#define TCL_2          2 /* Assign tasks in TCL_2 to processor 2 */
#define TCL_3          3 /* Assign tasks in TCL_3 to processor 3 */
#define TCL_4          4 /* Assign tasks in TCL_4 to processor 4 */

#define TCL_1_ONLY     5 /* The same but the task cannot migrate */
#define TCL_2_ONLY     6 /* The same but the task cannot migrate */
#define TCL_3_ONLY     7 /* The same but the task cannot migrate */
#define TCL_4_ONLY     8 /* The same but the task cannot migrate */
#define TCL_SYSTIM_PRC 9 /* Used for global system timer event handler */

/*
 *  サポートする機能の定義
 */
#define TOPPERS_TARGET_SUPPORT_ENA_INT			/* ena_intをサポートする */
#define TOPPERS_TARGET_SUPPORT_DIS_INT			/* dis_intをサポートする */
#ifdef TOPPERS_SYSTIM_LOCAL
#define TOPPERS_TARGET_SUPPORT_GET_UTM			/* get_utmをサポートする */
#endif /* TOPPERS_SYSTIM_LOCAL */

/*
 *  Customのアイドル処理を使用
 */
#define TOPPERS_CUSTOM_IDLE

/*
 *  割込み優先度の範囲
 */
#define TMIN_INTPRI   (-15)   /* 割込み優先度の最小値（最高値）*/
#define TMAX_INTPRI   (-1)    /* 割込み優先度の最大値（最低値）*/

/*
 *  タイムティックの定義
 */
#define TIC_NUME  1U   /* タイムティックの周期の分子 */
#define TIC_DENO  1U   /* タイムティックの周期の分母 */

#ifndef TOPPERS_MACRO_ONLY
#if __TARGET_ARCH_ARM == 7
/*
 *  性能評価用の関数
 *  BCM2836/BCM2837に搭載されている Core Timer を使用する．
 */

/* 性能計測用のカウンタのデータ型 */
typedef uint64_t PERFCNT;

/*
 *  パフォーマンスカウンタの初期化
 */
Inline void
x_init_pcc(void)
{
	/* カウント停止はない */

	/* プリスケーラの設定 */
	sil_wrw_mem((void *)BCM283X_CTIMER_PRE, 1);

	/* カウンターレジスタの初期化 */
	sil_wrw_mem((void *)BCM283X_CTIMER_LS32B, 0);
	sil_wrw_mem((void *)BCM283X_CTIMER_MS32B, 0);

	/* コンペアバリューレジスタの初期化はしない */

	/* カウント開始はない */
}

/*
 *  パフォーマンスカウンタの読み込み
 */
Inline void
x_get_pcc(PERFCNT *p_count)
{
	uint32_t count_l;
	uint32_t count_h_p, count_h;

	/*
	 * 32bitカウンタ2個で64bitカウンタを実現されているため，
	 * 桁上げを考慮した読み込みとする．
	 */
	count_h_p = sil_rew_mem((void *)BCM283X_CTIMER_MS32B);
	count_l = sil_rew_mem((void *)BCM283X_CTIMER_LS32B);
	count_h = sil_rew_mem((void *)BCM283X_CTIMER_MS32B);
	if (count_h_p != count_h) {
		count_l = sil_rew_mem((void *)BCM283X_CTIMER_LS32B);
	}

	*p_count = count_h;
	*p_count = *p_count << 32;
	*p_count += count_l;
}

/*
 *  パフォーマンスカウンタのリセット
 */
Inline void
x_rst_pcc(void)
{
	/* カウンタ停止はない */

	/* カウンターレジスタの初期化 */
	sil_wrw_mem((void *)BCM283X_CTIMER_LS32B, 0);
	sil_wrw_mem((void *)BCM283X_CTIMER_MS32B, 0);

	/* カウンタ再開はない */
}

/*
 *  カウンタ値のnsecへの変換
 */
Inline uint64_t
x_cnv_nsec(PERFCNT count) {
	return (count * (1000U / BCM283X_CTIMER_CLOCK_FREQ_MHZ));
}

#endif /* __TARGET_ARCH_ARM == 7 */
#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS__KERNEL_H */
