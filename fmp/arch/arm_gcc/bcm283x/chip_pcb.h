/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2009-2011 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_pcb.h 955 2016-10-17 00:00:00Z azo $
 */

/*
 *  プロセッサ依存プロセッサコントロールブロックモジュール（BCM283X用）
 */
#ifndef TOPPERS_CHIP_PCB_H
#define TOPPERS_CHIP_PCB_H

#include "bcm283x.h"

/*
 *  コア依存モジュール（ARM用）
 */
#include "arm_gcc/common/core_pcb.h"

/*
 *  カーネルロックの方式
 */
#define TTYPE_KLOCK G_KLOCK /* Giant lock method */

/*
 *  Use the thread ID register to store a pointer to the PCB
 */
#define USE_THREAD_ID_PCB

#ifndef TOPPERS_MACRO_ONLY

/*
 *  プロセッサコントロールブロック
 */
typedef struct target_processor_control_block{
	/*
	 *  割込みハンドラテーブルへのポインタ
	 */
	const FP* p_inh_tbl;

	/*
	 *  ARM依存プロセッサコントロールブロック
	 */
	APCB arm_pcb;
}TPCB;

/*
 *  ロックの型
 */
typedef uint32_t LOCK;

#ifdef USE_THREAD_ID_PCB

#define OMIT_GET_MY_P_PCB

Inline PCB*
get_my_p_pcb(void)
{
	PCB* my_p_pcb;
	uint32_t tmp;

	CPU15_PONLY_THREAD_ID_READ(tmp);
	my_p_pcb = (PCB*)tmp;

	return my_p_pcb;
}
#endif /* USE_THREAD_ID_PCB */

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CHIP_PCB_H */
