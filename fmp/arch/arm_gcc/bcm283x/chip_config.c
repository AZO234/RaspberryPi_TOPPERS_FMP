/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2012 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_config.c 955 2016-10-17 00:00:00Z azo $
 */

/*
 * チップ依存モジュール（BCM283X用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "chip_serial.h"
#include "mp.h"
#include <stdlib.h>
#include "target_ipi.h"

#ifdef TOPPERS_ENA_PERF
extern PERFCNT perf_boot_time[];
#endif /* TOPPERS_ENA_PERF */

#ifdef TOPPERS_ENABLE_TRACE
#include <stdio.h>
static FILE *trace_log_file;
#endif /* TOPPERS_ENABLE_TRACE */

const FP _kernel_inh_table[TNUM_INH];

/*
 *  SIL_LOC_SPN()を取得したプロセッサの管理
 */
uint32_t TOPPERS_spn_var;

#ifdef USE_IPI_DIS_HANDER_BYPASS
/*
 *  終了処理開始フラグ
 */
static bool_t ext_ker_reqflg;
#endif /* USE_IPI_DIS_HANDER_BYPASS */

/*
 * 各割込みの割込み要求禁止フラグの状態
 */
uint32_t idf[4];

/*
 * 現在の割込み優先度マスクの値
 */
PRI ipm;

/*
 *  割込み属性が設定されているかを判別するための変数
 */
uint32_t bitpat_cfgintb;
uint32_t bitpat_cfgint1;
uint32_t bitpat_cfgint2;
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
uint32_t bitpat_cfgintc0;
#endif

#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
/*
 *  現在の割込みの有効フラグの状態
 *  割込みの有効状態を示すレジスタが無いのでこのフラグで表現する．
 */
volatile uint32_t bcm283x_enable_intflag;
#endif

/*
 *  str_ker() の実行前にマスタプロセッサのみ実行される初期化処理
 */
void
chip_mprc_initialize(void)
{
        /*
         * Mailboxを初期化
         */
	sil_wrw_mem((void *)BCM283X_C0MB_INTCTL, 0x1U);
	sil_wrw_mem((void *)BCM283X_C1MB_INTCTL, 0x2U);
	sil_wrw_mem((void *)BCM283X_C2MB_INTCTL, 0x4U);
	sil_wrw_mem((void *)BCM283X_C3MB_INTCTL, 0x8U);
	sil_wrw_mem((void *)BCM283X_C0MB_0RC, 0xFFFFFFFU);
	sil_wrw_mem((void *)BCM283X_C1MB_1RC, 0xFFFFFFFU);
	sil_wrw_mem((void *)BCM283X_C2MB_2RC, 0xFFFFFFFU);
	sil_wrw_mem((void *)BCM283X_C3MB_3RC, 0xFFFFFFFU);

	/*
	 *  SILのスピンロック用変数の初期化
	 */
	TOPPERS_spn_var = 0U;

	/*
	 * 各割込みの割込み要求禁止フラグ全禁止
	 */
	idf[0] = ~0U;
	idf[1] = ~0U;
	idf[2] = ~0U;
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	idf[3] = ~0U;
#endif

	/*
	 * 割込み優先度マスクの値は最低（全割り込み許可）
	 */ 
	ipm = TMAX_INTPRI;

	/*
	 * 全ての割込みをマスク
	 */ 
	bcm283x_disable_int(0,~0U);
	bcm283x_disable_int(1,~0U);
	bcm283x_disable_int(2,~0U);
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	bcm283x_disable_int(3,~0U);
#endif

	/*
	 * 全ての割込み要因をクリア
	 */
	bcm283x_clear_int(0,~0U);
	bcm283x_clear_int(1,~0U);
	bcm283x_clear_int(2,~0U);
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	bcm283x_clear_int(3,~0U);
#endif

	/*
	 *  割込み属性が設定されているかを判別するための変数を初期化する．
	 */
	bitpat_cfgintb = 0U;
	bitpat_cfgint1 = 0U;
	bitpat_cfgint2 = 0U;
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	bitpat_cfgintc0 = 0U;
#endif

#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	/*
	 *  割込み有効フラグを初期化する．
	 */
	bcm283x_enable_intflag = 0U;
#endif

#ifdef USE_IPI_DIS_HANDER_BYPASS
	/*
	 *  終了処理開始フラグの初期化
	 */
	ext_ker_reqflg = false;
#endif /* USE_IPI_DIS_HANDER_BYPASS */
}

/*
 *  ベクターテーブル（chip_support.S）
 */
extern void *vector_table;

/*
 *  チップ依存の初期化
 */
void
chip_initialize(void)
{
	TPCB *p_tpcb = get_my_p_tpcb();

#ifdef USE_THREAD_ID_PCB
	/*
	 *  Thread ID レジスタへのPCBへのポインタの設定
	 */
	CPU15_PONLY_THREAD_ID_WRITE((uint32_t)p_pcb_table[x_prc_index()]);
#endif /* USE_THREAD_ID_PCB */

	p_tpcb = get_my_p_tpcb();

	/*
	 * ARM依存の初期化
	 */
	core_initialize();

	/*
	 * キャッシュを無効に
	 */
	mpcore_cache_disable();

	/*
	 * MMUを有効に
	 */
	mpcore_mmu_init();

	/*
	 * SMPモードへ
	 */
	mpcore_smp_mode_enable();

	/*
	 *  ブランチプリディクタを有効化
	 */
	btac_enable();

	/*
	 * キャッシュを有効に
	 */
	mpcore_cache_enable();

	/*
	 *  割込みハンドラテーブルへのポインタの初期化
	 */
	p_tpcb->p_inh_tbl = _kernel_inh_table;

#ifndef OMIT_VBAR_SETUP
	CP15_SET_VBAR((uint32_t) &vector_table);
#endif /* OMIT_VBAR_SETUP */

#ifdef TOPPERS_ENA_PERF
	if (x_sense_mprc()) {
		x_get_pcc(&perf_boot_time[2]);
		x_rst_pcc();
	}
#endif /* TOPPERS_ENA_PERF */
}

/*
 *  チップ依存の終了処理
 */
void
chip_exit(void)
{
	/*
	 *  すべての割込みをマスクする．
	 */
	bcm283x_disable_int(0,~0U);
	bcm283x_disable_int(1,~0U);
	bcm283x_disable_int(2,~0U);
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	bcm283x_disable_int(3,~0U);
#endif

	/*
	 *  ARM依存の終了処理
	 */
	core_terminate();
}

/*
 *  実行時間分布集計モジュールで用いる測定用タイマの初期化関数
 */
void
perf_timer_initialize(intptr_t exinf)
{
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  FMPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．FI4カーネルに利用する場合には，エラーを返すようにすべき
 *  であろう．
 *
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask)
{
	assert(VALID_INTNO_CFGINT(ID_PRC(x_prc_index()), intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/* 
	 * いったん割込みを禁止する
	 */    
	x_disable_int(intno);

	if(INTNO_MASK(intno) >= 0 && INTNO_MASK(intno) < 32) {
		/*
		 *  割込み属性が設定されているかを判別するための変数の設定
		 */
		bitpat_cfgintb |= INTNO_BITPAT(INTNO_MASK(intno));
	} else if(INTNO_MASK(intno) >= 32 && INTNO_MASK(intno) < 64) {
		bitpat_cfgint1 |= INTNO_BITPAT(INTNO_MASK(intno) - 32);
	} else if(INTNO_MASK(intno) >= 64 && INTNO_MASK(intno) < 96) {
		bitpat_cfgint2 |= INTNO_BITPAT(INTNO_MASK(intno) - 64);
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	} else if(INTNO_MASK(intno) >= 96 && INTNO_MASK(intno) < 128) {
		bitpat_cfgintc0 |= INTNO_BITPAT(INTNO_MASK(intno) - 96);
#endif
	}

	if ((intatr & TA_ENAINT) != 0U){
		(void)x_enable_int(intno);
	}    
}

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 * 未定義の割込みが入った場合の処理
 */
void
default_int_handler(void){
	ID prc_id = ID_PRC(x_prc_index());

	syslog_1(LOG_EMERG, "Processor %d : Unregistered Interrupt occurs.", prc_id);
	target_exit();
}
#endif /* OMIT_DEFAULT_INT_HANDLER */


#ifdef USE_IPI_DIS_HANDER_BYPASS

/*
 *  カーネル終了処理要求
 */
void
ext_ker_request(void)
{
	ID prcid;
	volatile int i;

	/* すでに要求が出ていればリターン */
	if (ext_ker_reqflg) {
		return;
	}

	ext_ker_reqflg = true;

	for(prcid = 1; prcid <= TNUM_PRCID; prcid++){
		if (prcid != ID_PRC(x_prc_index())) {
			dic_swi((1<<(prcid-1)), IPINO_EXT);
			/*
			 * dic_swi()を連続して発行すると割込みを正しく付けないプロセッ
			 * サがいるためループを挿入
			 */
			for(i = 0; i < 10; i++);
		}
	}
}

/*
 *  カーネル終了処理用のプロセッサ間割込みハンドラ
 */
void
ipi_ext_handler(void)
{
	ext_ker();
}
#endif /* USE_IPI_DIS_HANDER_BYPASS */
