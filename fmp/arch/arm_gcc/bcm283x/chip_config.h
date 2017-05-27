/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2011 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_config.h 955 2016-10-17 00:00:00Z azo $
 */

/*
 *  チップ依存モジュール（BCM283X用）
 *
 *  カーネルのチップ依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_CHIP_CONFIG_H
#define TOPPERS_CHIP_CONFIG_H

/*
 *  チップ依存部ののハードウェア資源の定義
 */
#include "bcm283x.h"

#ifdef USE_IPI_DIS_HANDER_BYPASS
#define OMIT_KER_REQ_ON_IPI

#ifndef TOPPERS_MACRO_ONLY
/*
 *  カーネル終了処理要求
 */
extern void ext_ker_request(void);
#endif /* TOPPERS_MACRO_ONLY */
#endif /* USE_IPI_DIS_HANDER_BYPASS */

/*
 *  割込み番号のマスク（下位16bitのみが有効）
 */
#define INTNO_MASK(intno) (intno & 0xffffU)

/*
 *  割込み番号の接続プロセッサのプロセッサID（上位16bit）
 */
#define INTNO_PRCID(intno) (intno >> 16U)

/*
 *  割込みハンドラ番号のマスク（下位16bitのみが有効）
 */
#define INHNO_MASK(intno) (intno & 0xffffU)

/*
 *  割込みハンドラ番号と割込み番号に関する定義
 */
#define TMIN_INTNO  BCM283X_TMIN_INTNO
#define TMAX_INTNO  BCM283X_TMAX_INTNO
#define TNUM_INT    BCM283X_TNUM_INT

#define TMIN_INHNO  TMIN_INTNO
#define TMAX_INHNO  TMAX_INTNO
#define TNUM_INH    TNUM_INT

#ifndef TOPPERS_MACRO_ONLY

/*
 * 各割込みの割込み要求禁止フラグの状態
 */
extern uint32_t idf[4];

/*
 *  割込み優先度マスク操作ライブラリ
 *
 *  AT91SAM7Sは割込み優先度マスクをIRC内でハードウェア的に持つが，
 *  ソフトウェア側から値を読み書きできないため，割込み要求禁止フラ
 *  グにより割込み優先度マスクを実現する
 */

/*
 *  現在の割込み優先度マスクの値
 */
extern PRI ipm;

/*
 *  割込み優先度マスク毎にセットする，割込み要求禁止フラグの値
 *  のテーブル
 */
extern const uint32_t _kernel_ipm_mask_table[TNUM_INTPRI][4];

/*
 *  割込み属性が設定されているかを判別するための変数
 */
extern uint32_t	bitpat_cfgintb;
extern uint32_t	bitpat_cfgint1;
extern uint32_t	bitpat_cfgint2;
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
extern uint32_t	bitpat_cfgintc0;
#endif

#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
/*
 *  現在の割込みの有効フラグの状態
 *  割込みの有効状態を示すレジスタが無いのでこのフラグで表現する．
 */
extern volatile uint32_t bcm283x_enable_intflag;
#endif

/*
 *  自プロセッサのAPCBへのポインタの取得
 *  インクルード順序の関係上，target_pcb.h には記述できないためここで定義する．
 */
Inline APCB*
get_my_p_apcb(void)
{
	return &((get_my_p_tpcb())->arm_pcb);
}

/*
 *  ロック関係
 */

/*
 *  割込み番号の範囲の判定
 */
#if TMIN_INTNO == 0
#define VALID_INTNO(prcid, intno) ((INTNO_MASK(intno) <= TMAX_INTNO) \
                                    && (INTNO_PRCID(intno) == 0U || INTNO_PRCID(intno) == prcid))
#else /* !TMIN_INTNO == 0 */
#define VALID_INTNO(prcid, intno) (((TMIN_INTNO <= (INTNO_MASK(intno)) && (INTNO_MASK(intno)) <= TMAX_INTNO)) \
                                    && (INTNO_PRCID(intno) == 0U || INTNO_PRCID(intno) == prcid))
#endif /* TMIN_INTNO == 0 */

#define VALID_INTNO_DISINT(prcid, intno)	VALID_INTNO(prcid, intno)
#define VALID_INTNO_CFGINT(prcid, intno)	VALID_INTNO(prcid, intno)

/*
 *  割込みハンドラの登録用テーブル（kernel_cfg.c）
 */
extern const FP* const p_inh_table[TNUM_PRCID];

/*
 *  割込みハンドラの設定
 *
 *  割込みハンドラ番号inhnoの割込みハンドラの起動番地をinthdrに設定する
 */
Inline void
x_define_inh(INHNO inhno, FP int_entry, uint_t affinity_mask)
{
#if TMIN_INTNO == 0
		assert((INHNO_MASK(inhno) <= TMAX_INHNO));
#else /* !TMIN_INTNO == 0 */
	assert(((TMIN_INHNO <= INHNO_MASK(inhno)) && (INHNO_MASK(inhno) <= TMAX_INHNO)));
#endif /* TMIN_INTNO == 0 */
}

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INT_ENTRY(inhno, inthdr)    inthdr
#define INTHDR_ENTRY(inhno, inhno_num, inthdr)

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  割込み優先度マスク操作ライブラリ
 *
 *  DICでは，DIC内の優先度レジスタにCPUの割込み優先度マスク（IPM）を設定する．
 *  割込み優先度マスクは値が大きいほど低優先度である．
 *  割込み優先度マスクはの段階はARM11とCA9で異なる．
 *    ・ARM11(ARMv6) : 16段階（0x00 〜 0x0f）
 *    ・CA9(ARMv7)   : 32段階（0x00 〜 0x1f）
 *  IPMを保存しておくために，割込み優先度の外部表現（-1から連続した負の値）
 *  を使うことも可能であるが，余計な計算が必要になる．これを避けるため，IPMを
 *  保存する場合には，DICで扱う優先度の形式とする．この値を割込み優先度マスク
 *  の内部表現と呼び，IIPMと書くことにする．
 */

/*
 *  割込み優先度マスクのビットマスク
 */
#ifndef TOPPERS_MACRO_ONLY
#define IPM_BIT_MASK  0x1fU
#else /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_MASK  0x1f
#endif /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_OFFSET 3

/*
 *  割込み優先度マスクの外部表現と内部表現の変換
 *
 *  アセンブリ言語のソースファイルからインクルードする場合のために，型
 *  キャストしない定義も用意している．
 */
#ifndef TOPPERS_MACRO_ONLY
#define EXT_IPM(iipm)    ((PRI)(iipm - IPM_BIT_MASK))    /* 内部表現を外部表現に */
#define INT_IPM(ipm)    ((uint8_t)(ipm + IPM_BIT_MASK))  /* 外部表現を内部表現に */
#else /* TOPPERS_MACRO_ONLY */
#define EXT_IPM(iipm)    (iipm - IPM_BIT_MASK)           /* 内部表現を外部表現に */
#define INT_IPM(ipm)    (ipm + IPM_BIT_MASK)             /* 外部表現を内部表現に */
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  TIPM_ENAALL（割込み優先度マスク全解除）の内部表現
 */
#define IIPM_ENAALL        (INT_IPM(TIPM_ENAALL))

#ifndef TOPPERS_MACRO_ONLY
/*
 * 割込み要求のマスク
 */
Inline void
bcm283x_disable_int(uint32_t no, uint32_t mask)
{
	switch(no) {
	case 0: 
		sil_wrw_mem((void *)(AIC_DISABLE_BASIC), mask);
		break;
	case 1: 
		sil_wrw_mem((void *)(AIC_DISABLE_IRQ1), mask);
		break;
	case 2: 
		sil_wrw_mem((void *)(AIC_DISABLE_IRQ2), mask);
		break;
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	case 3: 
		bcm283x_enable_intflag &= ~(mask);
		break;
#endif
	}
}

/*
 * 割込み要求のマスクの解除
 */
Inline void
bcm283x_enable_int(uint32_t no, uint32_t mask)
{
	switch(no) {
	case 0: 
		sil_wrw_mem((void *)(AIC_ENABLE_BASIC), mask);
		break;
	case 1:
		sil_wrw_mem((void *)(AIC_ENABLE_IRQ1), mask);
		break;
	case 2: 
		sil_wrw_mem((void *)(AIC_ENABLE_IRQ2), mask);
		break;
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	case 3:
		bcm283x_enable_intflag |= mask;
		break;
#endif
	}
}

/*
 * 割込み要求のクリア
 */
Inline void
bcm283x_clear_int(uint32_t no, uint32_t mask)
{
}

/*
 * 割込み要求のチェック
 */
Inline bool_t
bcm283x_probe_int(uint32_t intno)
{
	if(INTNO_MASK(intno) >= 0 && INTNO_MASK(intno) < 32) {
		return(sil_rew_mem((void *)(AIC_BASIC)) & (1U <<  INTNO_MASK(intno)      ));
	} else if(INTNO_MASK(intno) >= 32 && INTNO_MASK(intno) < 64) {
		return(sil_rew_mem((void *)(AIC_PEND1)) & (1U << (INTNO_MASK(intno) - 32)));
	} else if(INTNO_MASK(intno) >= 64 && INTNO_MASK(intno) < 96) {
		return(sil_rew_mem((void *)(AIC_PEND2)) & (1U << (INTNO_MASK(intno) - 64)));
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	} else if(INTNO_MASK(intno) >= 96 && INTNO_MASK(intno) < 127) {
		return(sil_rew_mem((void *)(BCM283X_C0_IRQSOURCE)) & (1U << (INTNO_MASK(intno) - 96)));
#endif
	}
	return(false);
}

/*
 *  IPM（ハードウェアの割込み優先度マスク，内部表現）の現在値の読出し
 */
Inline uint8_t
current_iipm(void)
{
//	return(dic_cpu_current_priority());
	return(0);
}

/*
 *  IPM（ハードウェアの割込み優先度マスク，内部表現）の現在値の設定
 */
Inline void
set_iipm(uint8_t iipm)
{
//	dic_cpu_set_priority(iipm);
}

/*
 * (モデル上の)割込み優先度マスクの設定
 */
Inline void
x_set_ipm(PRI intpri)
{
	set_iipm(INT_IPM(intpri));
}

#define t_set_ipm(intpri) x_set_ipm(intpri)
#define i_set_ipm(intpri) x_set_ipm(intpri)

/*
 *  (モデル上の)割込み優先度マスクの参照
 */
Inline PRI
x_get_ipm(void)
{
	return(EXT_IPM(current_iipm()));
}

#define t_get_ipm() x_get_ipm()
#define i_get_ipm() x_get_ipm()

/*
 *  割込み属性アクセステーブル
 *
 *  割込み属性テーブルへのポインタ．割込み属性テーブルは，割込み属性が設定
 *  されていれば"1"，設定されていなければ"0"となる
 */
//extern const uint8_t* const p_cfgint_table[];

//#define my_cfgint_table (p_cfgint_table[x_prc_index()])

/*
 * （モデル上の）割込み要求禁止フラグのセット
 *
 *  指定された，割込み番号の割込み要求禁止フラグのセットして，割込みを
 *  禁止する．
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline bool_t
x_disable_int(INTNO intno)
{
	if(INTNO_MASK(intno) >= 0 && INTNO_MASK(intno) < 32) {
		if ((bitpat_cfgintb & INTNO_BITPAT((INTNO_MASK(intno)))) == 0U) {
		        return(false);
		}
		bcm283x_disable_int(0,INTNO_BITPAT((INTNO_MASK(intno))));
		idf[0] |= INTNO_BITPAT((intno));
	} else if(INTNO_MASK(intno) >= 32 && INTNO_MASK(intno) < 64) {
		if ((bitpat_cfgint1 & INTNO_BITPAT((INTNO_MASK(intno) - 32))) == 0U) {
		        return(false);
		}
		bcm283x_disable_int(1,INTNO_BITPAT((INTNO_MASK(intno) - 32)));
		idf[1] |= INTNO_BITPAT((intno - 32));
	} else if(INTNO_MASK(intno) >= 64 && INTNO_MASK(intno) < 96) {
		if ((bitpat_cfgint1 & INTNO_BITPAT((INTNO_MASK(intno) - 64))) == 0U) {
		        return(false);
		}
		bcm283x_disable_int(2,INTNO_BITPAT((INTNO_MASK(intno) - 64)));
		idf[2] |= INTNO_BITPAT((intno - 64));
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	} else if(INTNO_MASK(intno) >= 96 && INTNO_MASK(intno) < 128) {
		if ((bitpat_cfgint1 & INTNO_BITPAT((INTNO_MASK(intno) - 96))) == 0U) {
		        return(false);
		}
		bcm283x_disable_int(3,INTNO_BITPAT((INTNO_MASK(intno) - 96)));
		idf[3] |= INTNO_BITPAT((intno - 96));
#endif
	}

	return(true);
}

#define t_disable_int(intno)  x_disable_int(intno)
#define i_disable_int(intno)  t_disable_int(intno)

/*
 *  (モデル上の)割り要求禁止フラグの解除
 *
 *  指定された，割込み番号の割込み要求禁止フラグのクリアして，割込みを
 *  許可する．
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline bool_t
x_enable_int(INTNO intno)
{
    uint32_t ipm_maskb = _kernel_ipm_mask_table[(~ipm) + 1][0];
    uint32_t ipm_mask1 = _kernel_ipm_mask_table[(~ipm) + 1][1];
    uint32_t ipm_mask2 = _kernel_ipm_mask_table[(~ipm) + 1][2];
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
    uint32_t ipm_maskc0 = _kernel_ipm_mask_table[(~ipm) + 1][3];
#endif

	if(INTNO_MASK(intno) >= 0 && INTNO_MASK(intno) < 32) {
		idf[0] &= ~INTNO_BITPAT((INTNO_MASK(intno)));
	} else if(INTNO_MASK(intno) >= 32 && INTNO_MASK(intno) < 64) {
		idf[1] &= ~INTNO_BITPAT((INTNO_MASK(intno) - 32));
	} else if(INTNO_MASK(intno) >= 64 && INTNO_MASK(intno) < 96) {
		idf[2] &= ~INTNO_BITPAT((INTNO_MASK(intno) - 64));
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	} else if(INTNO_MASK(intno) >= 96 && INTNO_MASK(intno) < 128) {
		idf[3] &= ~INTNO_BITPAT((INTNO_MASK(intno) - 96));
#endif
	}

    /* 全割込み禁止 */
    bcm283x_disable_int(0,~0U);    
    bcm283x_disable_int(1,~0U);    
    bcm283x_disable_int(2,~0U);    
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
    bcm283x_disable_int(3,~0U);    
#endif
    /* マスク指定されていない割込みのみ許可 */
    bcm283x_enable_int(0,~(ipm_maskb|idf[0]));
    bcm283x_enable_int(1,~(ipm_mask1|idf[1]));
    bcm283x_enable_int(2,~(ipm_mask2|idf[2]));
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
    bcm283x_enable_int(3,~(ipm_maskc0|idf[3]));
#endif

	return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)

/*
 * 割込み要求のクリア
 */
Inline void
x_clear_int(INTNO intno)
{
	if(INTNO_MASK(intno) >= 0 && INTNO_MASK(intno) < 32) {
		bcm283x_clear_int(0, INTNO_BITPAT((INTNO_MASK(intno))));
	} else if(INTNO_MASK(intno) >= 32 && INTNO_MASK(intno) < 64) {
		bcm283x_clear_int(1, INTNO_BITPAT((INTNO_MASK(intno) - 32)));
	} else if(INTNO_MASK(intno) >= 64 && INTNO_MASK(intno) < 96) {
		bcm283x_clear_int(2, INTNO_BITPAT((INTNO_MASK(intno) - 64)));
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	} else if(INTNO_MASK(intno) >= 96 && INTNO_MASK(intno) < 128) {
		bcm283x_clear_int(3, INTNO_BITPAT((INTNO_MASK(intno) - 96)));
#endif
	}
}

#define t_clear_int(intno) x_clear_int(intno)
#define i_clear_int(intno) x_clear_int(intno)

/*
 *  割込み要求のチェック
 */
Inline bool_t
x_probe_int(INTNO intno)
{
	return(bcm283x_probe_int(intno));
}

#define t_probe_int(intno) x_probe_int(intno)
#define i_probe_int(intno) x_probe_int(intno)

/*
 *  割込み要求ラインの属性の設定
 *
 */
extern void x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask);

/*
 * 割込みハンドラの入り口で必要なIRC操作
 *
 */
Inline void
i_begin_int(INTNO intno)
{
}

/*
 * 割込みハンドラの出口で必要なIRC操作
 */
Inline void
i_end_int(INTNO intno)
{
}

/*
 *  str_ker() の前でマスタプロセッサで行う初期化
 */
extern void chip_mprc_initialize(void);

/*
 *  チップ依存の初期化
 */
extern void chip_initialize(void);

/*
 *  チップ依存部の終了
 *
 *  システムを終了する時に使う．
 */
extern void chip_exit(void);

/*
 *  割込みハンドラ（chip_support.S）
 */
extern void interrupt_handler(void);

/*
 *  未定義の割込みが入った場合の処理
 */
extern void default_int_handler(void);

extern int bcm283x_getmycoreindex(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  ARM依存モジュール
 */
#include "arm_gcc/common/core_config.h"

/*
 *  チップ依存モジュールのツール依存部
 */
#include <chip_config_tool.h>

#endif /* TOPPERS_CHIP_CONFIG_H */
